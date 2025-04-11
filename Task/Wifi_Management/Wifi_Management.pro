TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $$PWD/bins

SOURCES += \
        WiFiManagement.c \
        main.c

HEADERS += \
    WiFiManagement.h \
    wifi.h
