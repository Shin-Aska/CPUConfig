QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    coreinfo.cpp \
    cpuinfo.cpp \
    main.cpp \
    cpuconfig.cpp \
    utility.cpp

HEADERS += \
    coreinfo.h \
    cpuconfig.h \
    cpuinfo.h \
    utility.h

FORMS += \
    cpuconfig.ui

TRANSLATIONS += \
    CPUMaster_en_US.ts

INCLUDEPATH += \
    /usr/include/KF5 \
    /usr/include/KF5/KAuth \
    /usr/include/KF5/KCoreAddons \
    /usr/include/KF5/KAuthCore \
    /usr/include/sensors

LIBS += \
    -l:libKF5CoreAddons.so.5 \
    -l:libKF5Auth.so \
    -l:libKF5AuthCore.so \
    -l:libsensors.so

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
