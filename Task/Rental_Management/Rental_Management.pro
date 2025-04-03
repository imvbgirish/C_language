TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $$PWD/bins

SOURCES += \
        bookingDetails.c \
        main.c \
        manageLogin.c \
        vehicleManagement.c

HEADERS += \
    adminDetails.h \
    bookingDetails.h \
    customerDetails.h \
    manageLogin.h \
    personDetails.h \
    rentVehicleDetails.h \
    rentalBikes.h \
    rentalCars.h \
    vehicleManagement.h
