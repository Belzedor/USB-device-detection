QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.

win32: LIBS += $$PWD/libusb-MinGW-x64/lib/libusb-1.0.a

INCLUDEPATH += $$PWD/libusb-MinGW-x64/include/libusb-1.0
DEPENDPATH += $$PWD/libusb-MinGW-x64/include/libusb-1.0


