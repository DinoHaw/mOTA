QT       += core gui serialport
RC_ICONS = icon.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_NO_WARNING_OUTPUT\ QT_NO_DEBUG_OUTPUT

SOURCES += \
    crc.cpp \
    main.cpp \
    mainwindow.cpp \
    serial_port.cpp \
    ymodem.cpp

HEADERS += \
    crc.h \
    mainwindow.h \
    serial_port.h \
    ymodem.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
