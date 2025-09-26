import os
from dotenv import load_dotenv
import tkinter as tk
from tkinter import ttk, messagebox
import pandas as pd
from datetime import datetime, date, timedelta
from kiteconnect import KiteConnect, KiteTicker
import json
import numpy as np
import math
import sqlite3
import shutil

# --- Dependency Check ---
try:
    from scipy.signal import find_peaks
except ImportError:
    print("FATAL ERROR: This version of the bot requires the 'scipy' library.")
    print("Please install it by running: pip install scipy")
    exit(1)

# --- Cross-Platform Sound Alerts ---
try:
    import simpleaudio as sa
    def generate_tone(frequency, duration_ms):
        sample_rate = 44100
        t = np.linspace(0, duration_ms / 1000, int(sample_rate * duration_ms / 1000), False)
        audio = np.sin(frequency * t * 2 * np.pi)
        audio *= 32767 / np.max(np.abs(audio))
        audio = audio.astype(np.int16)
        return audio
    def play_sound(frequency, duration_ms):
        try:
            tone = generate_tone(frequency, duration_ms)
            sa.play_buffer(tone, 1, 2, 44100)
        except Exception as e:
            print(f"Could not play sound: {e}")
            print('\a', end='')
    def play_entry_sound(): play_sound(1200, 200)
    def play_profit_sound(): play_sound(1500, 300)
    def play_loss_sound(): play_sound(300, 500)
    def play_warning_sound(): play_sound(400, 800)
except ImportError:
    print("Warning: 'simpleaudio' not found. For full audio alerts, run: pip install simpleaudio")
    def play_entry_sound(): print('\a', end='')
    def play_profit_sound(): print('\a', end='')
    def play_loss_sound(): print('\a', end='')
    def play_warning_sound(): print('\a', end='')

# --- API Authentication & Config ---
load_dotenv()
API_KEY = os.getenv("API_KEY")
API_SECRET = os.getenv("API_SECRET")
kite = KiteConnect(api_key=API_KEY)
access_token = None

INDEX_CONFIG = {
    "NIFTY": {"name": "NIFTY", "token": 256265, "symbol": "NSE:NIFTY 50", "strike_step": 50, "exchange": "NFO"},
    "BANKNIFTY": {"name": "BANKNIFTY", "token": 260105, "symbol": "NSE:NIFTY BANK", "strike_step": 100, "exchange": "NFO"},
    "SENSEX": {"name": "SENSEX", "token": 265, "symbol": "BSE:SENSEX", "strike_step": 100, "exchange": "BFO"}
}

MARKET_STANDARD_PARAMS = {
    'wma_period': 9, 'sma_period': 9, 'rsi_period': 9, 'rsi_signal_period': 3,
    'rsi_angle_lookback': 2, 'rsi_angle_threshold': 12.5, 'atr_period': 14,
    'min_atr_value': 2.5, 'ma_gap_threshold_pct': 0.5
}

def save_access_token(token):
    with open("access_token.json", "w") as f:
        json.dump({"access_token": token, "date": datetime.now().strftime("%Y-%m-%d")}, f)

def load_access_token():
    try:
        with open("access_token.json", "r") as f:
            data = json.load(f)
            if data["date"] == datetime.now().strftime("%Y-%m-%d"):
                return data["access_token"]
    except FileNotFoundError:
        return None

def authenticate():
    global access_token
    access_token = load_access_token()
    if access_token:
        try:
            kite.set_access_token(access_token)
            kite.profile()
            print("Authentication successful using saved token.")
            return
        except Exception as e:
            print(f"Saved token is invalid: {e}. Manual authentication required.")

    print(f"==== MANUAL AUTHENTICATION REQUIRED ====\n    https://kite.zerodha.com/connect/login?api_key={API_KEY}")
    request_token = input("Paste your request_token here: ").strip()
    try:
        session = kite.generate_session(request_token, api_secret=API_SECRET)
        access_token = session["access_token"]
        kite.set_access_token(access_token)
        save_access_token(access_token)
        print("Authentication successful and new token saved.")
    except Exception as e:
        print(f"Authentication failed: {e}")
        exit(1)

# --- Indicator Helpers ---
def calculate_wma(series, length=9):
    if len(series) < length: return pd.Series(index=series.index, dtype=float)
    weights = np.arange(1, length + 1)
    return series.rolling(length).apply(lambda x: np.dot(x, weights) / weights.sum(), raw=True)

def calculate_rsi(series, length=9):
    if len(series) < length: return pd.Series(index=series.index, dtype=float)
    delta = series.diff()
    gain = (delta.where(delta > 0, 0)).ewm(alpha=1/length, adjust=False).mean()
    loss = (-delta.where(delta < 0, 0)).ewm(alpha=1/length, adjust=False).mean()
    loss = loss.replace(0, 1e-10)
    rs = gain / loss
    return 100 - (100 / (1 + rs))

def calculate_atr(high, low, close, length=14):
    if len(close) < length: return pd.Series(index=close.index, dtype=float)
    tr = pd.concat([high - low, np.abs(high - close.shift()), np.abs(low - close.shift())], axis=1).max(axis=1)
    return tr.ewm(alpha=1/length, adjust=False).mean()

# --- Database Setup ---
def setup_database(db_path='trading_data.db'):
    conn = sqlite3.connect(db_path)
    cursor = conn.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS trades (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            timestamp TEXT NOT NULL,
            trigger_reason TEXT NOT NULL,
            symbol TEXT,
            pnl REAL,
            entry_price REAL,
            exit_price REAL,
            exit_reason TEXT,
            trend_state TEXT,
            atr REAL
        )
    ''')
    conn.commit()
    conn.close()

class Strategy:
    def __init__(self, params, ui_callbacks, selected_index="NIFTY"):
        self.params = params
        self.ui = ui_callbacks
        self.config = INDEX_CONFIG[selected_index]
        self.db_path = 'trading_data.db'
        self.trading_mode = self.params.get('trading_mode', 'Paper Trading')
        self.aggressiveness = self.params.get('aggressiveness', 'Conservative')

        self.partial_profit_trigger_pct = self.params.get('partial_profit_trigger_pct', 20.0)
        self.partial_exit_pct = self.params.get('partial_exit_pct', 50.0)

        self.index_name = self.config["name"]
        self.index_token = self.config["token"]
        self.index_symbol = self.config["symbol"]
        self.strike_step = self.config["strike_step"]
        self.exchange = self.config["exchange"]

        self.token_to_symbol = {self.index_token: self.index_symbol}
        self.prices, self.price_history = {}, {}
        self.option_instruments = self.load_instruments()
        self.last_used_expiry = self.get_weekly_expiry()

        self.position = None
        self.daily_pnl = 0
        self.daily_profit = 0
        self.daily_loss = 0
        self.trade_log = []
        self.performance_stats = {'total_trades': 0, 'winning_trades': 0, 'losing_trades': 0}
        self.daily_trade_limit_hit = False

        self.data_df = pd.DataFrame(columns=['open', 'high', 'low', 'close', 'sma', 'wma', 'rsi', 'rsi_sma', 'atr'])
        self.current_minute, self.current_candle = None, {}
        self.option_candles = {}
        self.pending_steep_signal, self.trend_state = None, None
        self.uoa_watchlist = {}
        self.trades_this_minute = 0
        self.initial_subscription_done = False

        self.analysis_log = []
        self.trend_candle_count = 0
        self.partial_exit_done = False

        self.last_analysis_time = datetime.now()
        self.last_exit_time = None

        try:
            with open('strategy_params.json', 'r') as f: self.STRATEGY_PARAMS = json.load(f)
            self._log_debug("System", "Loaded optimized strategy parameters.")
        except FileNotFoundError:
            self._log_debug("Warning", "strategy_params.json not found. Using defaults.")
            self.STRATEGY_PARAMS = MARKET_STANDARD_PARAMS.copy()

    def _log_debug(self, source, message):
        self.ui['log_debug']((datetime.now().strftime('%H:%M:%S'), source, message))

    def log_trade_decision(self, trade_info):
        conn = sqlite3.connect(self.db_path)
        cursor = conn.cursor()
        atr_value = self.data_df.iloc[-1]['atr'] if not self.data_df.empty and 'atr' in self.data_df.columns else 0
        trade_info['atr'] = round(float(atr_value) if pd.notna(atr_value) else 0.0, 2)
        columns = ', '.join(trade_info.keys()); placeholders = ', '.join('?' * len(trade_info))
        sql = f'INSERT INTO trades ({columns}) VALUES ({placeholders})'
        cursor.execute(sql, tuple(trade_info.values())); conn.commit(); conn.close()

    def load_instruments(self):
        try:
            instruments = [i for i in kite.instruments(self.exchange) if i.get('name') == self.index_name and i.get('instrument_type') in ['CE', 'PE']]
            self._log_debug("Instruments", f"Loaded {len(instruments)} {self.index_name} instruments.")
            return instruments
        except Exception as e: self._log_debug("Instruments", f"Error loading instruments: {e}"); return []

    def get_weekly_expiry(self):
        today = date.today()
        future_expiries = sorted([i['expiry'] for i in self.option_instruments if i.get('expiry') and i['expiry'] >= today])
        return future_expiries[0] if future_expiries else None

    def bootstrap_data(self):
        try:
            data = kite.historical_data(self.index_token, datetime.now() - timedelta(days=7), datetime.now(), "minute")
            df = pd.DataFrame(data).tail(200); df.index = pd.to_datetime(df['date'])
            self.data_df = self._calculate_indicators(df); self._update_trend_state()
            self._log_debug("Bootstrap", f"Historical data loaded. {len(self.data_df)} candles.")
        except Exception as e: self._log_debug("Bootstrap", f"Could not bootstrap data: {e}")

    def _calculate_indicators(self, df):
        df = df.copy()
        df['sma'] = df['close'].rolling(window=self.STRATEGY_PARAMS['sma_period']).mean()
        df['wma'] = calculate_wma(df['close'], length=self.STRATEGY_PARAMS['wma_period'])
        df['rsi'] = calculate_rsi(df['close'], length=self.STRATEGY_PARAMS['rsi_period'])
        df['rsi_sma'] = df['rsi'].rolling(window=self.STRATEGY_PARAMS['rsi_signal_period']).mean()
        df['atr'] = calculate_atr(df['high'], df['low'], df['close'], length=self.STRATEGY_PARAMS['atr_period'])
        return df

    def _update_trend_state(self):
        if len(self.data_df) < self.STRATEGY_PARAMS.get('sma_period', 9): return
        last = self.data_df.iloc[-1]
        if pd.isna(last['wma']) or pd.isna(last['sma']): return
        current_state = 'BULLISH' if last['wma'] > last['sma'] else 'BEARISH'
        if self.trend_state != current_state:
            self.trend_state = current_state; self.trend_candle_count = 1
            self.ui['update_trend'](self.trend_state); self._log_debug("Trend", f"Trend is now {self.trend_state} (MA Crossover)")
        else: self.trend_candle_count += 1

    def update_price_history(self, symbol, price):
        self.price_history.setdefault(symbol, []).append((datetime.now(), price))
        if len(self.price_history[symbol]) > 20: self.price_history[symbol].pop(0)

    def update_candle_and_indicators(self, ltp, symbol=None):
        self.current_minute = datetime.now().replace(second=0, microsecond=0)
        is_index = (symbol is None or symbol == self.index_symbol)

        candle_dict = self.current_candle if is_index else self.option_candles.setdefault(symbol, {})

        if candle_dict.get('minute') != self.current_minute:
            self.trades_this_minute = 0
            if is_index and 'minute' in candle_dict:
                new_row = pd.DataFrame([candle_dict], index=[candle_dict['minute']])
                self.data_df = pd.concat([self.data_df, new_row]).tail(200)
                self.data_df = self._calculate_indicators(self.data_df)
                self._update_trend_state()
                self.run_analysis_and_checks()

            candle_dict.update({'minute': self.current_minute, 'open': ltp, 'high': ltp, 'low': ltp, 'close': ltp})

        else:
            candle_dict.update({
                'high': max(candle_dict.get('high', ltp), ltp),
                'low': min(candle_dict.get('low', ltp), ltp),
                'close': ltp
            })

        now = datetime.now()
        if is_index and (now - self.last_analysis_time).total_seconds() > 3:
            self.last_analysis_time = now
            self.run_intra_candle_analysis()

    def run_intra_candle_analysis(self):
        if self.position: return
        if self.trades_this_minute >= 4: return
        if len(self.data_df) < 3 or 'open' not in self.current_candle: return
        live_candle = self.current_candle
        prev_candle = self.data_df.iloc[-1]
        prev_candle_2 = self.data_df.iloc[-2]
        self.check_live_candle_patterns(live_candle, prev_candle, prev_candle_2)

    def run_analysis_and_checks(self):
        self.analyze_price_action()
        if self.pending_steep_signal: self.check_pending_steep_signal()
        self.check_trade_entry()

    def check_trade_entry(self):
        if self.trades_this_minute >= 4:
            if datetime.now().second % 15 == 0: self._log_debug("Risk", "Max trade actions (4) for this minute reached.")
            return

        daily_sl = self.params.get('daily_sl', 0); daily_pt = self.params.get('daily_pt', 0)
        if self.daily_trade_limit_hit:
            if datetime.now().second % 30 == 0: self._log_debug("System", "Daily trade limit hit. No new trades.")
            return
        if daily_sl < 0 and self.daily_pnl <= daily_sl: self.daily_trade_limit_hit = True; play_warning_sound(); self._log_debug("RISK", f"Daily Stop-Loss of {daily_sl} hit. Trading disabled."); return
        if daily_pt > 0 and self.daily_pnl >= daily_pt: self.daily_trade_limit_hit = True; play_profit_sound(); self._log_debug("RISK", f"Daily Profit Target of {daily_pt} hit. Trading disabled."); return
        if self.position or self.data_df.empty or len(self.data_df) < 50: return

        log_this_minute = datetime.now().second % 15 == 0
        if self.check_uoa_entry(log_this_minute): return
        if self.check_trend_continuation(log_this_minute): return
        if self.check_ma_crossover_trade(log_this_minute): return
        if self.check_candle_pattern_entry(log_this_minute): return
        if self.check_rsi_immediate_entry(log_this_minute): return
        self.check_steep_reentry()

    def get_entry_option(self, side, strike=None):
        spot = self.prices.get(self.index_symbol)
        if not spot: return None
        if strike is None: strike = self.strike_step * round(spot / self.strike_step)
        for o in self.option_instruments:
            if o.get('expiry') == self.last_used_expiry and o.get('strike') == strike and o.get('instrument_type') == side: return o
        return None

    def is_price_rising(self, symbol):
        history = self.price_history.get(symbol, [])
        if len(history) < 2: return False
        return history[-1][1] > history[-2][1]

    def is_price_falling(self, symbol):
        history = self.price_history.get(symbol, [])
        if len(history) < 3: return True
        return history[-1][1] < history[-2][1]

    def _is_opposite_falling(self, side, strike):
        opposite_side = 'PE' if side == 'CE' else 'CE'
        opposite_opt = self.get_entry_option(opposite_side, strike)
        if not opposite_opt:
            self._log_debug("Validation", f"Could not find opposite option for strike {strike} to validate.")
            return True
        opposite_symbol = opposite_opt['tradingsymbol']
        if opposite_symbol not in self.prices:
            self._log_debug("Validation", f"No live price for opposite option {opposite_symbol}.")
            return True
        if not self.is_price_falling(opposite_symbol):
            self._log_debug("Trade Rejected", f"Opposite option {opposite_symbol} is not falling.")
            return False
        return True

    def _is_accelerating(self, symbol, lookback_ticks=5, acceleration_factor=2.0):
        history = self.price_history.get(symbol, [])
        if len(history) < lookback_ticks: return False
        recent_ticks = history[-lookback_ticks:]
        time_diff = (recent_ticks[-1][0] - recent_ticks[-2][0]).total_seconds()
        if time_diff <= 0: return False
        current_price = recent_ticks[-1][1]; previous_price = recent_ticks[-2][1]
        current_velocity = (current_price - previous_price) / time_diff
        if current_velocity <= 0: return False
        avg_time_diff = (recent_ticks[-1][0] - recent_ticks[0][0]).total_seconds()
        if avg_time_diff <= 0: return False
        avg_velocity = (current_price - recent_ticks[0][1]) / avg_time_diff
        if current_velocity > avg_velocity * acceleration_factor:
            self._log_debug("Momentum", f"{symbol} detected ACCELERATION.")
            return True
        return False

    def _momentum_ok(self, side, opt_sym, look=3):
        idx_history = self.price_history.get(self.index_symbol, []); opt_history = self.price_history.get(opt_sym, [])
        if len(idx_history) < look or len(opt_history) < look: return False
        idx_prices = [p[1] for p in idx_history]; opt_prices = [p[1] for p in opt_history]
        idx_up = sum(1 for i in range(1, look) if idx_prices[-i] > idx_prices[-i - 1])
        opt_up = sum(1 for i in range(1, look) if opt_prices[-i] > opt_prices[-i - 1])
        if side == 'CE': return idx_up >= 1 and opt_up >= 1
        else: idx_dn = (look - 1) - idx_up; return idx_dn >= 1 and opt_up >= 1

    # --- NEW: Unified and prioritized validation gauntlet ---
    def _validate_entry_conditions(self, side, opt):
        """
        Runs a prioritized, unified gauntlet of validation checks.
        Returns True if all conditions are met, otherwise False.
        """
        if not opt:
            return False

        symbol = opt['tradingsymbol']
        strike = opt['strike']

        # Priority 1: Is the option price rising right now? (Cheapest check)
        if not self.is_price_rising(symbol):
            return False

        # Priority 2: Is the opposite option's price falling? (Conviction check)
        if not self._is_opposite_falling(side, strike):
            return False

        # Priority 3: Is the micro-trend momentum okay?
        if not self._momentum_ok(side, symbol):
            return False

        # Priority 4: Is the price accelerating? (Most expensive 'power' check)
        if not self._is_accelerating(symbol):
            return False

        self._log_debug("Validation", f"PASS: All entry conditions met for {symbol}.")
        return True

    def _is_bullish_engulfing(self, prev, last):
        if pd.isna(prev['open']) or 'open' not in last or pd.isna(last['open']): return False
        prev_body = abs(prev['close'] - prev['open']); last_body = abs(last['close'] - last['open'])
        return (prev['close'] < prev['open'] and last['close'] > last['open'] and last['close'] > prev['open'] and last['open'] < prev['close'] and last_body > prev_body * 0.8)

    def _is_bearish_engulfing(self, prev, last):
        if pd.isna(prev['open']) or 'open' not in last or pd.isna(last['open']): return False
        prev_body = abs(prev['close'] - prev['open']); last_body = abs(last['close'] - last['open'])
        return (prev['close'] > prev['open'] and last['close'] < last['open'] and last['open'] > prev['close'] and last['close'] < prev['open'] and last_body > prev_body * 0.8)

    def _is_morning_star(self, c1, c2, c3):
        if any(pd.isna(c['open']) for c in [c1, c2]) or 'open' not in c3 or pd.isna(c3['open']): return False
        b1=abs(c1['close']-c1['open']); b2=abs(c2['close']-c2['open']); b3=abs(c3['close']-c3['open'])
        return (c1['close'] < c1['open'] and b2 < b1*0.3 and c2['high'] < c1['close'] and c3['open'] > c2['high'] and c3['close'] > c3['open'] and b3 > b1*0.6)

    def _is_evening_star(self, c1, c2, c3):
        if any(pd.isna(c['open']) for c in [c1, c2]) or 'open' not in c3 or pd.isna(c3['open']): return False
        b1=abs(c1['close']-c1['open']); b2=abs(c2['close']-c2['open']); b3=abs(c3['close']-c3['open'])
        return (c1['close'] > c1['open'] and b2 < b1*0.3 and c2['low'] > c1['close'] and c3['open'] < c2['low'] and c3['close'] < c3['open'] and b3 > b1*0.6)

    def _is_hammer(self, c):
        if 'open' not in c or pd.isna(c['open']): return False
        body=abs(c['close']-c['open']);
        if body==0: return False
        lower=min(c['open'],c['close'])-c['low']; upper=c['high']-max(c['open'],c['close']); rng=c['high']-c['low']
        return (lower > body*2.5 and upper < body*0.5 and (min(c['open'], c['close']) - c['low']) > rng*0.6)

    def _is_hanging_man(self, c): return self._is_hammer(c)

    def _is_doji(self, c, tol=0.05):
        if 'open' not in c or pd.isna(c['open']): return False
        body=abs(c['close']-c['open']); rng=c['high']-c['low']
        if rng==0: return False
        up=c['high']-max(c['open'],c['close']); lo=min(c['open'],c['close'])-c['low']
        return (body/rng) < tol and up > 0 and lo > 0

    def check_live_candle_patterns(self, live_candle, prev_candle, prev_candle_2):
        if self.position: return
        pattern, side, confidence = None, None, 0

        if self._is_bullish_engulfing(prev_candle, live_candle): pattern, side, confidence = 'Bullish_Engulfing_Live', 'CE', 10
        elif self._is_bearish_engulfing(prev_candle, live_candle): pattern, side, confidence = 'Bearish_Engulfing_Live', 'PE', 10
        elif self._is_morning_star(prev_candle_2, prev_candle, live_candle): pattern, side, confidence = 'Morning_Star_Live', 'CE', 9
        elif self._is_evening_star(prev_candle_2, prev_candle, live_candle): pattern, side, confidence = 'Evening_Star_Live', 'PE', 9
        elif self._is_hammer(live_candle) and self.trend_state == 'BEARISH': pattern, side, confidence = 'Hammer_Live', 'CE', 8
        elif self._is_hanging_man(live_candle) and self.trend_state == 'BULLISH': pattern, side, confidence = 'Hanging_Man_Live', 'PE', 8

        if not pattern: return
        opt = self.get_entry_option(side)

        current_time = datetime.now().time()
        if not (datetime.strptime("09:15", "%H:%M").time() <= current_time <= datetime.strptime("15:10", "%H:%M").time()):
            return

        if self._validate_entry_conditions(side, opt):
            self._log_debug("Live Pattern", f"🎯 LIVE TRADE: {pattern} (Confidence: {confidence}/10)")
            self.take_trade(f"{pattern}", opt)
            return True
        return False

    def check_candle_pattern_entry(self, log=False):
        if len(self.data_df) < 3 or not self.trend_state: return False
        last = self.data_df.iloc[-1]
        pattern, side, confidence = None, None, 0

        if self._is_doji(last):
            if self.trend_state == 'BULLISH' and self.trend_candle_count >= 5: pattern, side, confidence = 'Doji_Reversal_Elite', 'PE', 7
            elif self.trend_state == 'BEARISH' and self.trend_candle_count >= 5: pattern, side, confidence = 'Doji_Reversal_Elite', 'CE', 7

        if not pattern: return False
        opt = self.get_entry_option(side)

        current_time = datetime.now().time()
        if not (datetime.strptime("09:15", "%H:%M").time() <= current_time <= datetime.strptime("15:10", "%H:%M").time()):
            if log: self._log_debug("Elite Pattern", f"{pattern} - Outside optimal trading hours")
            return False

        if self._validate_entry_conditions(side, opt):
            self._log_debug("Elite Pattern", f"🎯 ELITE TRADE (Confirmed): {pattern} (Confidence: {confidence}/10)")
            self.take_trade(f"{pattern}", opt)
            return True
        return False

    def check_trend_continuation(self, log=False):
        if not self.trend_state or len(self.data_df) < 2: return False
        if not self.trend_candle_count >= 1: return False
        prev_candle = self.data_df.iloc[-1]
        current_price = self.prices.get(self.index_symbol)
        if not current_price: return False

        side, trigger = None, None
        if self.trend_state == 'BULLISH' and current_price > prev_candle['high']: side, trigger = 'CE', 'Trend_Continuation_CE_Breakout'
        elif self.trend_state == 'BEARISH' and current_price < prev_candle['low']: side, trigger = 'PE', 'Trend_Continuation_PE_Breakout'

        if side:
            opt = self.get_entry_option(side)
            if self._validate_entry_conditions(side, opt):
                self.take_trade(trigger, opt)
                return True
        return False

    def check_ma_crossover_trade(self, log=False):
        if len(self.data_df) < 2: return False
        last, prev = self.data_df.iloc[-1], self.data_df.iloc[-2]
        if any(pd.isna(v) for v in [last['wma'], last['sma'], prev['wma'], prev['sma']]): return False

        side, trigger = None, None
        if prev['wma'] <= prev['sma'] and last['wma'] > last['sma'] and last['close'] > last['open']:
            side, trigger = 'CE', "MA_Crossover_CE"
        elif prev['wma'] >= prev['sma'] and last['wma'] < last['sma'] and last['close'] < last['open']:
            side, trigger = 'PE', "MA_Crossover_PE"

        if side:
            opt = self.get_entry_option(side)
            if self._validate_entry_conditions(side, opt):
                if log: self._log_debug("Crossover", f"Confirmed {trigger} signal.")
                self.take_trade(trigger, opt)
                return True
        return False

    def check_rsi_immediate_entry(self, log=False):
        if len(self.data_df) < self.STRATEGY_PARAMS['rsi_angle_lookback'] + 1: return False
        last, prev = self.data_df.iloc[-1], self.data_df.iloc[-2]
        if any(pd.isna(v) for v in [last['rsi'], prev['rsi'], last['rsi_sma'], prev['rsi_sma']]): return False
        angle = self._calculate_rsi_angle(); angle_thresh = self.STRATEGY_PARAMS['rsi_angle_threshold']

        side, trigger = None, None
        if last['rsi'] > last['rsi_sma'] and prev['rsi'] <= prev['rsi_sma'] and angle > angle_thresh:
            side, trigger = 'CE', 'RSI_Immediate_CE'
        elif last['rsi'] < last['rsi_sma'] and prev['rsi'] >= prev['rsi_sma'] and angle < -angle_thresh:
            side, trigger = 'PE', 'RSI_Immediate_PE'

        if side:
            opt = self.get_entry_option(side)
            if self._validate_entry_conditions(side, opt):
                self.take_trade(trigger, opt)
                return True
        return False

    def _calculate_rsi_angle(self):
        lookback = self.STRATEGY_PARAMS['rsi_angle_lookback']
        if len(self.data_df) < lookback + 1: return 0
        rsi_values = self.data_df['rsi'].iloc[-(lookback + 1):].values
        try: slope = np.polyfit(np.arange(len(rsi_values)), rsi_values, 1)[0]; return math.degrees(math.atan(slope))
        except (np.linalg.LinAlgError, ValueError): return 0

    def check_steep_reentry(self):
        if len(self.data_df) < 1: return False
        last = self.data_df.iloc[-1]
        is_bullish_candle = last['close'] > last['open']
        if last['wma'] > last['sma'] and not is_bullish_candle: self.pending_steep_signal = {'side': 'PE', 'reason': 'Reversal_PE'}
        if last['wma'] < last['sma'] and is_bullish_candle: self.pending_steep_signal = {'side': 'CE', 'reason': 'Reversal_CE'}
        return False

    def add_to_watchlist(self, side, strike):
        opt = self.get_entry_option(side, strike=strike)
        if opt:
            if opt['instrument_token'] in self.uoa_watchlist: return False
            self.uoa_watchlist[opt['instrument_token']] = {'symbol': opt['tradingsymbol'], 'type': side, 'strike': strike}
            self._log_debug("UOA", f"Added {opt['tradingsymbol']} to watchlist."); self.ui['update_uoa_list'](list(self.uoa_watchlist.values())); play_entry_sound()
            return True
        self._log_debug("UOA", f"Could not find {side} option for strike {strike}"); return False

    def check_uoa_entry(self, log=False):
        if not self.uoa_watchlist or datetime.now().second < 10: return False
        for token, data in list(self.uoa_watchlist.items()):
            symbol, side, strike = data['symbol'], data['type'], data['strike']
            trend_ok = (side == 'CE' and self.trend_state == 'BULLISH') or (side == 'PE' and self.trend_state == 'BEARISH')
            if self.aggressiveness == 'Conservative' and not trend_ok: continue

            option_candle = self.option_candles.get(symbol); current_price = self.prices.get(symbol)
            if not option_candle or 'open' not in option_candle or not current_price: continue
            if current_price <= option_candle['open']: continue

            opt = self.get_entry_option(side, strike=strike)
            if self._validate_entry_conditions(side, opt):
                self.take_trade("UOA_Entry", opt)
                del self.uoa_watchlist[token]
                self.ui['update_uoa_list'](list(self.uoa_watchlist.values()))
                return True
        return False

    def check_pending_steep_signal(self):
        if not self.pending_steep_signal or len(self.data_df) == 0: return
        signal, self.pending_steep_signal = self.pending_steep_signal, None
        side, reason = signal['side'], signal['reason']
        opt = self.get_entry_option(side)
        if self._validate_entry_conditions(side, opt):
            self.take_trade(reason, opt)

    def calculate_trade_qty(self, option_price, lot_size):
        capital = self.params.get('start_capital')
        if not all([capital, lot_size, option_price]) or option_price <= 0: return lot_size
        return max(1, math.floor(capital / (option_price * lot_size))) * lot_size

    def take_trade(self, trigger, opt):
        if self.position or not opt: return
        if self.last_exit_time and (datetime.now() - self.last_exit_time).total_seconds() < 5: return

        symbol, side, price, lot_size, strike = opt['tradingsymbol'], opt['instrument_type'], self.prices.get(opt['tradingsymbol']), opt.get('lot_size'), opt.get('strike')
        if price is None or price < 1.0 or lot_size is None:
            self._log_debug("Trade", f"Invalid price/lot_size for {symbol}: P={price}, L={lot_size}"); return
        qty = self.calculate_trade_qty(price, lot_size)
        if self.trading_mode == 'Live Trading':
            try:
                order_id = kite.place_order(tradingsymbol=symbol, exchange=self.exchange, transaction_type=kite.TRANSACTION_TYPE_BUY, quantity=qty, variety=kite.VARIETY_REGULAR, order_type=kite.ORDER_TYPE_MARKET, product=kite.PRODUCT_MIS)
                self._log_debug("LIVE TRADE", f"Placed BUY order for {qty} {symbol}. Order ID: {order_id}")
            except Exception as e:
                self._log_debug("LIVE TRADE ERROR", f"Order placement failed: {e}"); play_loss_sound(); return
        else:
            self._log_debug("PAPER TRADE", f"Simulating BUY order for {qty} {symbol}.")

        initial_sl = max(price - self.params['trailing_sl_points'], price * (1 - self.params['trailing_sl_percent'] / 100))
        self.position = {'symbol': symbol, 'entry_price': price, 'direction': side, 'qty': qty, 'entry_qty': qty, 'trail_sl': round(initial_sl, 1), 'max_price': price, 'trigger_reason': trigger, 'entry_time': datetime.now().strftime('%Y-%m-%d %H:%M:%S'), 'lot_size': lot_size, 'strike': strike}
        self.partial_exit_done = False
        self.trades_this_minute += 1
        self.performance_stats['total_trades'] += 1; play_entry_sound()
        self.ui['update_trade_status'](self.position); self._log_debug("Trade", f"Position: {symbol} @ {price} Qty: {qty} Trigger: {trigger}")

    def evaluate_exit_logic(self):
        if not self.position: return
        p = self.position
        ltp = self.prices.get(p['symbol'])
        if ltp is None: return

        if ltp > p['max_price']: p['max_price'] = ltp
        p['trail_sl'] = round(max(p['trail_sl'], max(p['max_price'] - self.params['trailing_sl_points'], p['max_price'] * (1 - self.params['trailing_sl_percent'] / 100))), 1)
        profit_pct = ((ltp - p['entry_price']) / p['entry_price']) * 100 if p['entry_price'] > 0 else 0
        self.ui['update_trade_stats']((ltp - p['entry_price']) * p['qty'], profit_pct, p['trail_sl'], p['max_price'])

        if not self.partial_exit_done and self.partial_profit_trigger_pct > 0 and profit_pct >= self.partial_profit_trigger_pct:
            qty_based_on_pct = math.ceil((p['entry_qty'] * self.partial_exit_pct / 100) / p['lot_size']) * p['lot_size']
            qty_to_exit = max(p['lot_size'], qty_based_on_pct)
            qty_to_exit = min(qty_to_exit, p['qty'])
            if qty_to_exit > 0:
                self.exit_position("Partial Profit", partial_qty=qty_to_exit)
                return

        if ltp <= p['trail_sl']:
            self.exit_position("Trailing SL")
            return

        if 'open' in self.current_candle and not self.data_df.empty:
            live_index_candle = self.current_candle
            prev_index_candle = self.data_df.iloc[-1]

            if p['direction'] == 'CE' and self._is_bearish_engulfing(prev_index_candle, live_index_candle):
                self._log_debug("Exit Logic", "Invalidation: Bearish Engulfing on index. Exiting CE.")
                self.exit_position("Invalidation: Bearish Engulfing")
                return
            elif p['direction'] == 'PE' and self._is_bullish_engulfing(prev_index_candle, live_index_candle):
                self._log_debug("Exit Logic", "Invalidation: Bullish Engulfing on index. Exiting PE.")
                self.exit_position("Invalidation: Bullish Engulfing")
                return

    def exit_position(self, reason, partial_qty=None):
        if not self.position: return
        p = self.position; exit_price = self.prices.get(p['symbol'], p['entry_price'])
        exit_qty = partial_qty if partial_qty is not None else p['qty']
        self.trades_this_minute += 1
        if not partial_qty: self.last_exit_time = datetime.now()

        if self.trading_mode == 'Live Trading':
            try:
                order_id = kite.place_order(tradingsymbol=p['symbol'], exchange=self.exchange, transaction_type=kite.TRANSACTION_TYPE_SELL, quantity=exit_qty, variety=kite.VARIETY_REGULAR, order_type=kite.ORDER_TYPE_MARKET, product=kite.PRODUCT_MIS)
                self._log_debug("LIVE TRADE", f"Placed SELL order for {exit_qty} {p['symbol']}. Order ID: {order_id}")
            except Exception as e:
                self._log_debug("LIVE TRADE ERROR", f"Exit order failed: {e}"); play_loss_sound(); return
        else:
            self._log_debug("PAPER TRADE", f"Simulating SELL order for {exit_qty} {p['symbol']}.")

        net_pnl = (exit_price - p['entry_price']) * exit_qty
        self.daily_pnl += net_pnl
        if net_pnl > 0:
            self.performance_stats['winning_trades'] += 1 if partial_qty is None else 0; self.daily_profit += net_pnl; play_profit_sound()
        else:
            self.performance_stats['losing_trades'] += 1 if partial_qty is None else 0; self.daily_loss += net_pnl; play_loss_sound()

        if partial_qty:
            self.position['qty'] -= exit_qty
            self.partial_exit_done = True
            self.position['trail_sl'] = max(self.position['trail_sl'], self.position['entry_price'])
            self._log_debug("Trade", f"Partially exited {exit_qty} of {p['symbol']}. New SL: {self.position['trail_sl']:.2f}")
            if self.position['qty'] <= 0: self.position = None
            self.ui['update_trade_status'](self.position)
        else:
            trade_entry = (p['symbol'], p['entry_time'], p['trigger_reason'], f"{p['entry_price']:.2f}", f"{exit_price:.2f}", f"{net_pnl:.2f}", reason)
            self.trade_log.append(trade_entry); self.ui['update_trade_log'](self.trade_log)
            self._log_debug("Trade", f"Exited {p['symbol']} at {exit_price} P&L: {net_pnl:.2f}. Reason: {reason}")
            log_info = {'timestamp': datetime.now().strftime('%Y-%m-%d %H:%M:%S'), 'trigger_reason': p['trigger_reason'], 'symbol': p['symbol'], 'pnl': round(net_pnl, 2), 'entry_price': p['entry_price'], 'exit_price': exit_price, 'exit_reason': reason, 'trend_state': self.trend_state}
            self.log_trade_decision(log_info); self.position = None; self.ui['update_trade_status'](None)

        self.ui['update_daily_performance'](self.daily_pnl, self.daily_profit, self.daily_loss, self.performance_stats['winning_trades'], self.performance_stats['losing_trades'])

    def manual_exit(self):
        if self.position: self.exit_position("Manual Exit")

    def get_all_option_tokens(self):
        spot = self.prices.get(self.index_symbol)
        if not spot: return [self.index_token]
        atm_strike = self.strike_step * round(spot / self.strike_step)
        strikes = [atm_strike + (i - 3) * self.strike_step for i in range(7)]
        tokens = {self.index_token}
        for strike in strikes:
            for side in ['CE', 'PE']:
                if (opt := self.get_entry_option(side, strike)): tokens.add(opt['instrument_token'])
        tokens.update(self.uoa_watchlist.keys()); return list(tokens)

    def map_option_tokens(self, tokens):
        for o in self.option_instruments:
            if o['instrument_token'] in tokens: self.token_to_symbol[o['instrument_token']] = o['tradingsymbol']
        self._log_debug("Tokens", f"Mapped {len(self.token_to_symbol)} symbols for websocket.")

    def get_strike_pairs(self, count=7):
        spot = self.prices.get(self.index_symbol)
        if not spot: return []
        atm_strike = self.strike_step * round(spot / self.strike_step)
        strikes = [atm_strike + (i - count // 2) * self.strike_step for i in range(count)]
        return [{"strike": s, "ce": self.get_entry_option('CE', s), "pe": self.get_entry_option('PE', s)} for s in strikes]

    def calculate_uoa_conviction_score(self, option_data, atm_strike):
        score = 0; v_oi_ratio = option_data.get('volume', 0) / (option_data.get('oi', 0) + 1)
        score += min(v_oi_ratio / 2.0, 5); score += min(option_data.get('change', 0) / 10.0, 5)
        strike_dist = abs(option_data['strike'] - atm_strike) / self.strike_step
        if strike_dist <= 2: score += 3
        elif strike_dist <= 4: score += 1
        return score

    def scan_for_unusual_activity(self):
        self._log_debug("Scanner", "Running UOA scan..."); spot = self.prices.get(self.index_symbol)
        if not spot: self._log_debug("Scanner", "Aborting scan: Index price unavailable."); return
        try:
            atm_strike = self.strike_step * round(spot / self.strike_step); scan_range = 5 if self.index_name == "NIFTY" else 8
            target_strikes = [atm_strike + (i * self.strike_step) for i in range(-scan_range, scan_range + 1)]
            target_options = [i for i in self.option_instruments if i['expiry'] == self.last_used_expiry and i['strike'] in target_strikes]
            if not target_options: self._log_debug("Scanner", "No target options found."); return
            quotes = kite.quote([opt['instrument_token'] for opt in target_options])
            found_count, CONVICTION_THRESHOLD = 0, 6.0
            for instrument, data in quotes.items():
                opt_details = next((opt for opt in target_options if opt['instrument_token'] == data['instrument_token']), None)
                if not opt_details: continue
                quote_data = {"volume": data.get('volume', 0), "oi": data.get('oi', 0), "change": data.get('change', 0), "strike": opt_details['strike']}
                score = self.calculate_uoa_conviction_score(quote_data, atm_strike)
                if score >= CONVICTION_THRESHOLD:
                    if self.add_to_watchlist(opt_details['instrument_type'], opt_details['strike']):
                        self._log_debug("Scanner", f"High conviction: {opt_details['tradingsymbol']} (Score: {score:.1f})."); found_count += 1
            if found_count > 0: self._log_debug("Scanner", f"Scan complete. Added {found_count} new UOA."); self.ui['resubscribe']()
            else: self._log_debug("Scanner", "Scan complete. No new high-conviction opportunities.")
        except Exception as e: self._log_debug("Scanner", f"Error during UOA scan: {e}")

    def _find_swing_high_low(self, lookback=15, prominence=1):
        df = self.data_df.tail(lookback * 3)
        if len(df) < lookback: return None, None
        highs, lows = df['high'], df['low']
        peak_indices, _ = find_peaks(highs, distance=5, prominence=prominence)
        trough_indices, _ = find_peaks(-lows, distance=5, prominence=prominence)
        last_high = highs.iloc[peak_indices[-1]] if len(peak_indices) > 0 else None
        last_low = lows.iloc[trough_indices[-1]] if len(trough_indices) > 0 else None
        return last_low, last_high

    def _calculate_fib_levels(self, low, high):
        if low is None or high is None or high == low: return {}
        diff = high - low
        return {'100%': high, '78.6%': high - 0.214 * diff, '61.8%': high - 0.382 * diff, '50%': high - 0.5 * diff, '38.2%': high - 0.618 * diff, '23.6%': high - 0.764 * diff, '0%': low}

    def analyze_price_action(self):
        if len(self.data_df) < 50: return
        last_low, last_high = self._find_swing_high_low()
        fib_levels = self._calculate_fib_levels(last_low, last_high)
        analysis_data = {"time": datetime.now().strftime('%H:%M:%S'), "swing_low": f"{last_low:.2f}" if last_low else "N/A", "swing_high": f"{last_high:.2f}" if last_high else "N/A", "fib_levels": {k: f"{v:.2f}" for k, v in fib_levels.items()}}
        self._check_hypothetical_signals(last_low, last_high, fib_levels)
        self.ui['update_analysis_data'](analysis_data)

    def _check_hypothetical_signals(self, last_low, last_high, fib_levels):
        if self.data_df.empty: return
        last_close = self.data_df.iloc[-1]['close']; atr = self.data_df.iloc[-1]['atr']
        if pd.isna(atr): return
        if self.trend_state == 'BULLISH' and last_high and last_close > last_high: self._log_hypothetical_trade("BOS_Bullish", "CE")
        if self.trend_state == 'BEARISH' and last_low and last_close < last_low: self._log_hypothetical_trade("BOS_Bearish", "PE")
        if self.trend_state == 'BULLISH' and last_low and last_close < last_low: self._log_hypothetical_trade("CHoCH_Bearish", "PE")
        if self.trend_state == 'BEARISH' and last_high and last_close > last_high: self._log_hypothetical_trade("CHoCH_Bullish", "CE")
        if fib_levels:
            if self.trend_state == 'BULLISH' and abs(last_close - fib_levels.get('61.8%', 0)) < (0.75 * atr): self._log_hypothetical_trade("Fib_61.8_Bullish", "CE")
            if self.trend_state == 'BEARISH' and abs(last_close - fib_levels.get('38.2%', 0)) < (0.75 * atr): self._log_hypothetical_trade("Fib_38.2_Bearish", "PE")

    def _log_hypothetical_trade(self, reason, side):
        opt = self.get_entry_option(side); entry_price = self.prices.get(opt['tradingsymbol']) if opt else None
        if not entry_price: return
        pnl_sim = (entry_price * 0.1) * (50 if self.index_name == "NIFTY" else 15)
        log_entry = (datetime.now().strftime('%H:%M:%S'), reason, side, f"{entry_price:.2f}", f"{pnl_sim:.2f}")
        if not any(e[1] == reason and abs((datetime.strptime(e[0], '%H:%M:%S') - datetime.now()).total_seconds()) < 120 for e in self.analysis_log[-5:]):
            self.analysis_log.append(log_entry); self.ui['update_analysis_log'](self.analysis_log)

# ... (OptimizerBot class remains unchanged)
class OptimizerBot:
    def __init__(self, db_path='trading_data.db', params_path='strategy_params.json'):
        self.db_path, self.params_path = db_path, params_path; self.justifications = []
        try: self.conn = sqlite3.connect(self.db_path)
        except Exception as e: print(f"Optimizer DB error: {e}"); self.conn = None

    def get_historical_data(self, days=60):
        if not self.conn: return pd.DataFrame()
        try:
            df = pd.read_sql_query(f"SELECT * FROM trades WHERE timestamp >= date('now', '-{days} days')", self.conn)
            print(f"Optimizer: Loaded {len(df)} trades from last {days} days.")
            return df
        except Exception as e: print(f"Optimizer DB read error: {e}"); return pd.DataFrame()

    def analyze_performance(self, df):
        if df.empty: return None
        results = df.groupby('trigger_reason').agg(total_trades=('pnl', 'count'), total_pnl=('pnl', 'sum'), winning_trades=('pnl', lambda x: (x > 0).sum())).reset_index()
        results['win_rate'] = (results['winning_trades'] / results['total_trades']) * 100
        print("\n--- Long-Term Performance ---"); print(results.round(2)); return results

    def find_optimal_parameters(self):
        df = self.get_historical_data()
        if df.empty: return None, ["No historical data to optimize."]
        analysis = self.analyze_performance(df)
        if analysis is None: return None, ["Analysis failed."]
        self.justifications.append("Optimization Report (Last 60 Days):")
        try:
            with open(self.params_path, 'r') as f: new_params = json.load(f)
        except FileNotFoundError: return None, ["strategy_params.json not found."]
        rsi_stats = analysis[analysis['trigger_reason'].str.contains('RSI', na=False)]
        if not rsi_stats.empty and rsi_stats['win_rate'].mean() < 50:
            old = new_params.get('rsi_angle_threshold', 12.5); new_params['rsi_angle_threshold'] = round(old * 1.1, 2)
            self.justifications.append(f"- RSI win rate <50%. Tightening angle: {old} -> {new_params['rsi_angle_threshold']}.")
        ma_stats = analysis[analysis['trigger_reason'].str.contains('Anticipate', na=False)]
        if not ma_stats.empty and ma_stats['win_rate'].mean() < 50:
            old = new_params.get('ma_gap_threshold_pct', 0.5); new_params['ma_gap_threshold_pct'] = round(old * 0.9, 4)
            self.justifications.append(f"- MA Crossover win rate <50%. Reducing gap: {old} -> {new_params['ma_gap_threshold_pct']}.")
        if analysis['total_trades'].sum() > 0 and (analysis['total_pnl'].sum() / analysis['total_trades'].sum()) < 0:
            old = new_params.get('min_atr_value', 2.5); new_params['min_atr_value'] = round(old * 1.05, 2)
            self.justifications.append(f"- Overall P&L negative. Increasing min ATR: {old} -> {new_params['min_atr_value']}.")
        if len(self.justifications) == 1: self.justifications.append("- No parameter changes needed. Performance stable.")
        return new_params, self.justifications

    def update_strategy_file(self, new_params):
        if new_params:
            with open(self.params_path, 'w') as f: json.dump(new_params, f, indent=4)
            print(f"Optimizer: Updated '{self.params_path}'.")

class ScrollableFrame(ttk.Frame):
    def __init__(self, container, *args, **kwargs):
        super().__init__(container, *args, **kwargs)
        canvas = tk.Canvas(self); scrollbar = ttk.Scrollbar(self, orient="vertical", command=canvas.yview)
        self.interior = ttk.Frame(canvas)
        self.interior.bind("<Configure>", lambda e: canvas.configure(scrollregion=canvas.bbox("all")))
        canvas.create_window((0, 0), window=self.interior, anchor="nw"); canvas.configure(yscrollcommand=scrollbar.set)
        canvas.pack(side="left", fill="both", expand=True); scrollbar.pack(side="right", fill="y")

class TradingApp(tk.Tk):
    def __init__(self):
        super().__init__(); self.title("Trading Bot v35.2 - Optimized Validation"); self.geometry("1450x950")
        self.strategy, self.kws = None, None
        self.param_vars = self.create_param_vars()
        self.run_startup_optimization(); self.create_widgets(); self.start_ui_updater()

    def create_param_vars(self):
        return {
            'selected_index': tk.StringVar(value="NIFTY"), 'trading_mode': tk.StringVar(value="Paper Trading"),
            'aggressiveness': tk.StringVar(value="Conservative"), 'start_capital': tk.IntVar(value=50000),
            'trailing_sl_points': tk.DoubleVar(value=5.0), 'trailing_sl_percent': tk.DoubleVar(value=10.0),
            'auto_scan_uoa': tk.BooleanVar(value=False), 'daily_sl': tk.IntVar(value=-2000), 'daily_pt': tk.IntVar(value=4000),
            'partial_profit_trigger_pct': tk.DoubleVar(value=20.0), 'partial_exit_pct': tk.DoubleVar(value=50.0)
        }

    def create_log_tree(self, parent, columns, height=10):
        frame = ttk.Frame(parent); frame.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        tree = ttk.Treeview(frame, columns=columns, show="headings", height=height)
        vsb = ttk.Scrollbar(frame, orient="vertical", command=tree.yview); hsb = ttk.Scrollbar(frame, orient="horizontal", command=tree.xview)
        tree.configure(yscrollcommand=vsb.set, xscrollcommand=hsb.set); vsb.pack(side='right', fill='y'); hsb.pack(side='bottom', fill='x'); tree.pack(side='left', fill='both', expand=True)
        for col in columns: tree.heading(col, text=col); tree.column(col, width=120, anchor='w')
        return tree

    def create_widgets(self):
        main_pane = tk.PanedWindow(self, orient=tk.HORIZONTAL, sashrelief=tk.RAISED); main_pane.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        left_scroll_frame = ScrollableFrame(main_pane); left_pane = left_scroll_frame.interior; main_pane.add(left_scroll_frame, minsize=420)
        right_pane = tk.Frame(main_pane); main_pane.add(right_pane)
        status_frame = ttk.LabelFrame(left_pane, text="Live Status"); status_frame.pack(fill=tk.X, pady=5, padx=5)
        self.conn_status_label = ttk.Label(status_frame, text="Status: DISCONNECTED", foreground="red", font=("Arial", 11, "bold")); self.conn_status_label.pack(pady=5, padx=5, anchor='w')
        self.mode_status_label = ttk.Label(status_frame, text="MODE: NOT STARTED", font=("Arial", 14, "bold")); self.mode_status_label.pack(pady=5)
        self.index_price_var = tk.StringVar(value="INDEX: --"); ttk.Label(status_frame, textvariable=self.index_price_var, font=("Arial", 18, "bold"), foreground="blue").pack(pady=5)
        self.trend_state_var = tk.StringVar(value="Trend: ---"); ttk.Label(status_frame, textvariable=self.trend_state_var, font=("Arial", 12, "bold")).pack(pady=2)
        param_frame = ttk.LabelFrame(left_pane, text="Parameters"); param_frame.pack(fill=tk.X, pady=5, padx=5)
        param_grid = tk.Frame(param_frame); param_grid.pack(pady=5, padx=5)
        params_config = [
            ("Index:", 'selected_index', list(INDEX_CONFIG.keys())), ("Mode:", 'trading_mode', ["Paper Trading", "Live Trading"]),
            ("Aggro:", 'aggressiveness', ["Conservative", "Moderate"]), ("Capital:", 'start_capital', None), ("SL (Pts):", 'trailing_sl_points', None),
            ("SL (%):", 'trailing_sl_percent', None), ("Daily SL:", 'daily_sl', None), ("Daily PT:", 'daily_pt', None),
            ("P-Exit Profit%:", 'partial_profit_trigger_pct', None), ("P-Exit Size%:", 'partial_exit_pct', None)
        ]
        for i, (label, key, values) in enumerate(params_config):
            ttk.Label(param_grid, text=label).grid(row=i % 5, column=(i // 5) * 2, sticky="w", pady=2)
            if values: w = ttk.Combobox(param_grid, textvariable=self.param_vars[key], values=values, state="readonly", width=12); setattr(self, f"{key}_dropdown", w)
            else: w = ttk.Entry(param_grid, width=14, textvariable=self.param_vars[key])
            w.grid(row=i % 5, column=(i // 5) * 2 + 1, pady=2, padx=5)

        ttk.Button(param_frame, text="Apply Changes", command=self.apply_parameters).pack(pady=5)
        control_button_frame = ttk.Frame(param_frame)
        control_button_frame.pack(pady=(5, 10))
        self.start_button = ttk.Button(control_button_frame, text="Start Trading Bot", command=self.start_bot)
        self.start_button.pack(side=tk.LEFT, padx=5)
        self.stop_button = ttk.Button(control_button_frame, text="Stop Bot", command=self.stop_bot, state='disabled')
        self.stop_button.pack(side=tk.LEFT, padx=5)

        # ... (rest of create_widgets method is unchanged)
        optimizer_frame = ttk.LabelFrame(left_pane, text="Intelligence"); optimizer_frame.pack(fill=tk.X, pady=5, padx=5)
        ttk.Button(optimizer_frame, text="Analyze & Optimize Now", command=self.run_manual_optimizer).pack(pady=5, fill=tk.X, padx=5)
        ttk.Checkbutton(optimizer_frame, text="Enable Auto-Scan for UOA", variable=self.param_vars['auto_scan_uoa']).pack(pady=2, padx=5, anchor='w')
        ttk.Button(optimizer_frame, text="Reset to Market Standards", command=self.reset_parameters_to_standard).pack(pady=5, fill=tk.X, padx=5)
        perf_frame = ttk.LabelFrame(left_pane, text="Daily Performance"); perf_frame.pack(fill=tk.X, pady=5, padx=5)
        self.total_pnl_var = tk.StringVar(value="Net P&L: ₹ 0.00"); ttk.Label(perf_frame, textvariable=self.total_pnl_var, font=("Arial", 12, "bold")).pack(anchor='w', padx=5)
        self.total_profit_var = tk.StringVar(value="Gross Profit: ₹ 0.00"); ttk.Label(perf_frame, textvariable=self.total_profit_var, foreground="green").pack(anchor='w', padx=5)
        self.total_loss_var = tk.StringVar(value="Gross Loss: ₹ 0.00"); ttk.Label(perf_frame, textvariable=self.total_loss_var, foreground="red").pack(anchor='w', padx=5)
        self.win_loss_var = tk.StringVar(value="Wins: 0 | Losses: 0"); ttk.Label(perf_frame, textvariable=self.win_loss_var).pack(anchor='w', padx=5, pady=(2,5))
        trade_status_frame = ttk.LabelFrame(left_pane, text="Current Trade"); trade_status_frame.pack(fill=tk.X, pady=5, padx=5)
        self.current_trade_var = tk.StringVar(value="STATUS: No Active Trade"); ttk.Label(trade_status_frame, textvariable=self.current_trade_var, font=("Arial", 11, "bold")).pack(pady=5, padx=5, anchor='w')
        stats_grid = tk.Frame(trade_status_frame); stats_grid.pack(fill=tk.X, padx=5, pady=5)
        self.pnl_var, self.profit_pct_var, self.trail_sl_var, self.max_price_var = tk.StringVar(value="P&L: ₹ --"), tk.StringVar(value="Profit: -- %"), tk.StringVar(value="Trail SL: --"), tk.StringVar(value="Max Price: --")
        ttk.Label(stats_grid, textvariable=self.pnl_var).grid(row=0, column=0, sticky='w'); ttk.Label(stats_grid, textvariable=self.profit_pct_var).grid(row=0, column=1, sticky='w', padx=10)
        ttk.Label(stats_grid, textvariable=self.trail_sl_var).grid(row=1, column=0, sticky='w'); ttk.Label(stats_grid, textvariable=self.max_price_var).grid(row=1, column=1, sticky='w', padx=10)
        ttk.Button(trade_status_frame, text="Manual Exit Trade", command=self.manual_exit_action).pack(pady=10)
        uoa_frame = ttk.LabelFrame(left_pane, text="UOA Watchlist"); uoa_frame.pack(fill=tk.X, pady=5, padx=5)
        uoa_input_frame = tk.Frame(uoa_frame); uoa_input_frame.pack(fill=tk.X, pady=5)
        ttk.Label(uoa_input_frame, text="Strike:").pack(side=tk.LEFT, padx=5); self.uoa_strike_entry = ttk.Entry(uoa_input_frame, width=10); self.uoa_strike_entry.pack(side=tk.LEFT, padx=5)
        ttk.Button(uoa_input_frame, text="Watch CE", command=lambda: self._add_to_watchlist_handler('CE')).pack(side=tk.LEFT, padx=2); ttk.Button(uoa_input_frame, text="Watch PE", command=lambda: self._add_to_watchlist_handler('PE')).pack(side=tk.LEFT, padx=2)
        self.uoa_tree = self.create_log_tree(uoa_frame, ("Symbol", "Type", "Strike"), height=3); self.uoa_tree.column("Symbol", width=150); self.uoa_tree.column("Type", width=50, anchor='center'); self.uoa_tree.column("Strike", width=80, anchor='center')
        right_notebook = ttk.Notebook(right_pane); right_notebook.pack(fill=tk.BOTH, expand=True)
        main_data_tab = ttk.Frame(right_notebook); right_notebook.add(main_data_tab, text="Market Data")
        option_frame = ttk.LabelFrame(main_data_tab, text="Option Chain"); option_frame.pack(fill=tk.X, pady=5); self.option_tree = self.create_log_tree(option_frame, ("LTP (CE)", "Strike", "LTP (PE)"), height=8)
        debug_frame = ttk.LabelFrame(main_data_tab, text="Debug Log"); debug_frame.pack(fill=tk.BOTH, expand=True, pady=5); self.debug_log_tree = self.create_log_tree(debug_frame, ("Time", "Source", "Message")); self.debug_log_tree.column("Time", width=80); self.debug_log_tree.column("Source", width=120); self.debug_log_tree.column("Message", width=500)
        trade_log_tab = ttk.Frame(right_notebook); right_notebook.add(trade_log_tab, text="Trade History"); self.trade_log_tree = self.create_log_tree(trade_log_tab, ("Symbol", "Entry Time", "Reason", "Entry", "Exit", "P&L", "Exit Reason"))
        for col, width in [("Symbol", 150), ("Entry Time", 130), ("Reason", 160), ("Entry", 80), ("Exit", 80), ("P&L", 90), ("Exit Reason", 140)]: self.trade_log_tree.column(col, width=width, anchor='w')
        analysis_tab = ttk.Frame(right_notebook); right_notebook.add(analysis_tab, text="Price Action Analysis")
        analysis_pane = tk.PanedWindow(analysis_tab, orient=tk.HORIZONTAL, sashrelief=tk.RAISED); analysis_pane.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        analysis_left = tk.Frame(analysis_pane, width=280); analysis_pane.add(analysis_left, minsize=250); analysis_right = tk.Frame(analysis_pane); analysis_pane.add(analysis_right)
        structure_frame = ttk.LabelFrame(analysis_left, text="Market Structure & Fibonacci"); structure_frame.pack(fill=tk.X, pady=5)
        self.swing_high_var = tk.StringVar(value="Swing High: --"); ttk.Label(structure_frame, textvariable=self.swing_high_var).pack(anchor='w', padx=5)
        self.swing_low_var = tk.StringVar(value="Swing Low: --"); ttk.Label(structure_frame, textvariable=self.swing_low_var).pack(anchor='w', padx=5)
        fib_levels = ['100%', '78.6%', '61.8%', '50%', '38.2%', '23.6%', '0%']; self.fib_level_vars = {lvl: tk.StringVar(value=f"{lvl}: --") for lvl in fib_levels}
        for level in fib_levels: ttk.Label(structure_frame, textvariable=self.fib_level_vars[level]).pack(anchor='w', padx=5)
        hypo_trade_frame = ttk.LabelFrame(analysis_right, text="Hypothetical Signals Log"); hypo_trade_frame.pack(fill=tk.BOTH, expand=True); self.analysis_log_tree = self.create_log_tree(hypo_trade_frame, ("Time", "Reason", "Side", "Entry", "Hypo. P&L"), height=18)

    def update_analysis_data(self, data):
        self.swing_high_var.set(f"Swing High: {data['swing_high']}"); self.swing_low_var.set(f"Swing Low: {data['swing_low']}")
        for level, var in self.fib_level_vars.items(): var.set(f"{level}: {data['fib_levels'].get(level, '--')}")

    def update_analysis_log(self, analysis_log):
        self.analysis_log_tree.delete(*self.analysis_log_tree.get_children())
        for log in reversed(analysis_log[-200:]): self.analysis_log_tree.insert("", 0, values=log)

    def reset_parameters_to_standard(self):
        if messagebox.askyesno("Confirm Reset", "Reset to market standards?"):
            try:
                with open('strategy_params.json', 'w') as f: json.dump(MARKET_STANDARD_PARAMS, f, indent=4)
                if self.strategy: self.strategy.STRATEGY_PARAMS = MARKET_STANDARD_PARAMS.copy(); self._log_debug("GUI", "Parameters reset.")
                messagebox.showinfo("Success", "Parameters reset.")
            except Exception as e: messagebox.showerror("Error", f"Failed to reset: {e}")

    def run_startup_optimization(self):
        try:
            optimizer = OptimizerBot(); optimized_params, justifications = optimizer.find_optimal_parameters()
            if optimized_params: optimizer.update_strategy_file(optimized_params); messagebox.showinfo("Startup Optimization", "Analysis complete.\n\n" + "\n".join(justifications))
            else: print("Startup: No historical data to optimize.")
        except Exception as e: print(f"Startup optimization error: {e}")

    def run_manual_optimizer(self):
        if messagebox.askyesno("Confirm Optimization", "Analyze last 60 days of trades and update parameters?"):
            try:
                optimizer = OptimizerBot(); optimized_params, justifications = optimizer.find_optimal_parameters()
                if optimized_params: optimizer.update_strategy_file(optimized_params); messagebox.showinfo("Optimization Complete", "Analysis complete.\n\n" + "\n".join(justifications))
                else: messagebox.showerror("Optimization Error", "Could not optimize. DB might be empty.")
            except Exception as e: messagebox.showerror("Optimization Error", str(e))

    def _add_to_watchlist_handler(self, side):
        if not self.strategy: messagebox.showerror("Error", "Bot not started."); return
        try:
            strike = int(self.uoa_strike_entry.get())
            if self.strategy.add_to_watchlist(side, strike): self.uoa_strike_entry.delete(0, tk.END); self.resubscribe_tokens()
        except (ValueError, TypeError): messagebox.showerror("Error", "Please enter a valid numeric strike.")

    def manual_exit_action(self):
        if self.strategy: self.strategy.manual_exit()

    def update_uoa_tree(self, watchlist_items):
        self.uoa_tree.delete(*self.uoa_tree.get_children())
        for item in watchlist_items: self.uoa_tree.insert("", "end", values=(item['symbol'], item['type'], item['strike']))

    def update_current_trade_display(self, position):
        if position: self.current_trade_var.set(f"Trade: {position['qty']} of {position['symbol']} @ {position['entry_price']:.2f}")
        else: self.current_trade_var.set("STATUS: No Active Trade"); self.update_gui_stats(None, None, None, None)

    def update_gui_stats(self, pnl, profit_pct, trail_sl, max_price):
        self.pnl_var.set(f"P&L: ₹ {pnl:,.2f}" if pnl is not None else "P&L: ₹ --")
        self.profit_pct_var.set(f"Profit: {profit_pct:.2f} %" if profit_pct is not None else "Profit: -- %")
        self.trail_sl_var.set(f"Trail SL: {trail_sl:.2f}" if trail_sl is not None else "Trail SL: --")
        self.max_price_var.set(f"Max Price: {max_price:.2f}" if max_price is not None else "Max Price: --")

    def update_trade_log_tree(self, trades):
        self.trade_log_tree.delete(*self.trade_log_tree.get_children())
        for trade in trades: self.trade_log_tree.insert("", "end", values=trade)

    def _log_debug(self, source, message): self.after(0, self.update_debug_log, (datetime.now().strftime('%H:%M:%S'), source, message))

    def update_debug_log(self, msg_tuple):
        self.debug_log_tree.insert("", 0, values=msg_tuple)
        if len(self.debug_log_tree.get_children()) > 400: self.debug_log_tree.delete(self.debug_log_tree.get_children()[-1])

    def start_ui_updater(self): self.update_option_chain_ui(); self.after(2000, self.start_ui_updater)

    def update_option_chain_ui(self):
        if not self.strategy: return
        spot = self.strategy.prices.get(self.strategy.index_symbol)
        if spot: self.index_price_var.set(f"{self.strategy.index_name}: {spot:.2f}")
        self.option_tree.delete(*self.option_tree.get_children())
        pairs = self.strategy.get_strike_pairs()
        if spot and pairs:
            for p in pairs:
                ce_ltp = self.strategy.prices.get(p['ce']['tradingsymbol'], "--") if p['ce'] else "--"
                pe_ltp = self.strategy.prices.get(p['pe']['tradingsymbol'], "--") if p['pe'] else "--"
                tag = 'atm' if abs(p["strike"] - spot) < self.strategy.strike_step / 2 else ('itm' if p["strike"] < spot else 'otm')
                self.option_tree.insert("", "end", values=(ce_ltp, p["strike"], pe_ltp), tags=(tag,))
        self.option_tree.tag_configure('itm', background='#E0FFE0'); self.option_tree.tag_configure('otm', background='#FFE0E0'); self.option_tree.tag_configure('atm', background='#FFFFE0')

    def start_bot(self):
        mode = self.param_vars['trading_mode'].get()
        if 'Live' in mode and not messagebox.askyesno("Confirm Live Trading", "Start bot in LIVE TRADING mode?"): return
        for widget_key in ['selected_index', 'trading_mode', 'aggressiveness']: getattr(self, f"{widget_key}_dropdown").config(state="disabled")
        self.start_button.config(state="disabled")
        self.stop_button.config(state="normal")
        self.update_mode_status_label(mode); self.current_trade_var.set("STATUS: No Active Trade")
        ui_callbacks = {'log_debug': self.update_debug_log, 'update_trend': lambda t: self.trend_state_var.set(f"Trend: {t}"), 'update_uoa_list': self.update_uoa_tree, 'update_trade_status': self.update_current_trade_display, 'update_trade_stats': self.update_gui_stats, 'update_trade_log': self.update_trade_log_tree, 'resubscribe': self.resubscribe_tokens, 'update_daily_performance': self.update_daily_performance, 'update_analysis_data': self.update_analysis_data, 'update_analysis_log': self.update_analysis_log}
        self.strategy = Strategy(params={k: v.get() for k, v in self.param_vars.items()}, ui_callbacks=ui_callbacks, selected_index=self.param_vars['selected_index'].get())
        self.strategy.bootstrap_data(); self._log_debug("System", f"Strategy instance created in {self.strategy.trading_mode} mode."); self.start_websocket_thread(); self.schedule_uoa_scanner()

    def stop_bot(self):
        if not self.strategy: return
        if messagebox.askyesno("Confirm Stop", "Are you sure you want to stop the bot? This will exit any open position."):
            self._log_debug("System", "User initiated stop. Shutting down...")
            if self.strategy.position: self.strategy.manual_exit()
            if self.kws and self.kws.is_connected(): self.kws.close(1000, "User requested stop")
            self.kws = None
            self.strategy = None
            self.start_button.config(state="normal")
            self.stop_button.config(state="disabled")
            for widget_key in ['selected_index', 'trading_mode', 'aggressiveness']: getattr(self, f"{widget_key}_dropdown").config(state="readonly")
            self.mode_status_label.config(text="MODE: NOT STARTED")
            self.trend_state_var.set("Trend: ---")
            self._log_debug("System", "Bot has been stopped.")

    def schedule_uoa_scanner(self):
        if self.strategy and self.param_vars['auto_scan_uoa'].get(): self.strategy.scan_for_unusual_activity()
        self.after(300000, self.schedule_uoa_scanner)

    def apply_parameters(self):
        if self.strategy:
            try:
                new_params = {k: v.get() for k, v in self.param_vars.items()}
                self.strategy.params.update(new_params)
                self.strategy.aggressiveness = new_params.get('aggressiveness', 'Conservative')
                self.strategy.partial_profit_trigger_pct = new_params.get('partial_profit_trigger_pct', 20.0)
                self.strategy.partial_exit_pct = new_params.get('partial_exit_pct', 50.0)
                self._log_debug("GUI", f"Parameters updated successfully.")
                messagebox.showinfo("Success", "Parameters updated.")
            except Exception as e: self._log_debug("GUI", f"Error applying parameters: {e}"); messagebox.showerror("Error", f"Could not apply parameters: {e}")

    def update_daily_performance(self, net_pnl, gross_profit, gross_loss, wins, losses):
        self.total_pnl_var.set(f"Net P&L: ₹ {net_pnl:,.2f}"); self.total_profit_var.set(f"Gross Profit: ₹ {gross_profit:,.2f}"); self.total_loss_var.set(f"Gross Loss: ₹ {gross_loss:,.2f}"); self.win_loss_var.set(f"Wins: {wins} | Losses: {losses}")

    def update_mode_status_label(self, mode):
        self.mode_status_label.config(text=f"MODE: {mode.upper()}", foreground="red" if 'Live' in mode else "green")

    def start_websocket_thread(self):
        self.kws = KiteTicker(API_KEY, access_token); self.kws.on_ticks = self.on_ticks; self.kws.on_connect = self.on_connect; self.kws.on_close = self.on_close; self.kws.on_error = self.on_error; self.kws.connect(threaded=True)

    def resubscribe_tokens(self):
        if self.strategy and self.kws and self.kws.is_connected():
            self._log_debug("WebSocket", "Resubscribing to tokens.")
            tokens = self.strategy.get_all_option_tokens()
            self.strategy.map_option_tokens(tokens)
            self.kws.subscribe(tokens); self.kws.set_mode(self.kws.MODE_LTP, tokens)

    def on_connect(self, ws, response):
        self.after(0, lambda: self.conn_status_label.config(text="Status: Connected", foreground="green"))
        if self.strategy:
            self._log_debug("WebSocket", f"Connected. Subscribing to index: {self.strategy.index_token}")
            ws.subscribe([self.strategy.index_token]); ws.set_mode(ws.MODE_LTP, [self.strategy.index_token])

    def on_error(self, ws, code, reason): self._log_debug("WebSocket", f"Error {code}: {reason}")
    def on_close(self, ws, code, reason):
        self.after(0, lambda: self.conn_status_label.config(text="Status: DISCONNECTED", foreground="red"))
        self._log_debug("WebSocket", f"Connection closed: {reason}")

    def on_ticks(self, ws, ticks):
        if not self.strategy: return
        self.after(0, self.handle_ticks, ticks)

    def handle_ticks(self, ticks):
        if not self.strategy: return
        if not self.strategy.initial_subscription_done and any(t.get('instrument_token') == self.strategy.index_token for t in ticks):
            self.strategy.prices[self.strategy.index_symbol] = next(t['last_price'] for t in ticks if t.get('instrument_token') == self.strategy.index_token)
            self._log_debug("WebSocket", "Index price received. Subscribing to full token list.")
            tokens = self.strategy.get_all_option_tokens()
            self.strategy.map_option_tokens(tokens)
            self.kws.subscribe(tokens); self.kws.set_mode(self.kws.MODE_LTP, tokens); self.strategy.initial_subscription_done = True
        for tick in ticks:
            token, ltp = tick.get('instrument_token'), tick.get('last_price')
            if token and ltp and (symbol := self.strategy.token_to_symbol.get(token)):
                self.strategy.prices[symbol] = ltp
                self.strategy.update_price_history(symbol, ltp)
                self.strategy.update_candle_and_indicators(ltp, symbol)
                if self.strategy.position and self.strategy.position['symbol'] == symbol: self.strategy.evaluate_exit_logic()

if __name__ == "__main__":
    if not os.path.exists('strategy_params.json'):
        with open('strategy_params.json', 'w') as f: json.dump(MARKET_STANDARD_PARAMS, f, indent=4)
    setup_database()
    authenticate()
    app = TradingApp()
    app.mainloop()

#Removed "One Trade Per Minute" Rule
#Added 5-Second Post-Exit Cooldown
#Max Two Full Trades Per Minute
#Option Price Rise Confirmation
#priority order change
#after partial exit SL is set to entry price
#stop button added
#opposite price falling