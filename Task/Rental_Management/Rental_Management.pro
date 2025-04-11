TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $$PWD/bins

SOURCES += \
        main.c \
        manageLogin.c \
        vehicleManagement.c

HEADERS += \
    adminDetails.h \
    bookingDetails.h \
    customerDetails.h \
    manageLogin.h \
    payment.h \
    personDetails.h \
    rentVehicleDetails.h \
    rentalBikes.h \
    rentalCars.h \
    vehicleManagement.h
