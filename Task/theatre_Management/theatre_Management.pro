TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $$PWD/bins

SOURCES += \
        main.c \
        manageAuditorium.c

HEADERS += \
    auditorium.h \
    calender.h \
    manageAuditorium.h
