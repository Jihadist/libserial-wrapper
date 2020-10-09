QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        helper_p.cpp \
        main.cpp \
        serialport.cpp \
        serialportinfo.cpp

INCLUDEPATH += $$PWD/libserialport

win32 {
    LIBS += -luser32
    contains(QMAKE_TARGET.arch, x86_64) {
       message("Windows x64 build")
       ## Windows x64 (64bit) specific build here
       CONFIG(debug, debug|release) {
       LIBS += -L$$PWD/libs/x64/ -llibserialport
       DESTDIR = $${PWD}/build/x64/debug
       } else {
       LIBS += -L$$PWD/libs/x64/ -llibserialport
       DESTDIR = $${PWD}/build/x64/release
       }
    } else {
        message("Windows x86 build")
        ## Windows x86 (32bit) specific build here
        CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/libs/x86/  -llibserialport
        DESTDIR = $${PWD}/build/x86/debug
        } else {
        LIBS += -L$$PWD/libs/x86/ -llibserialport
        DESTDIR = $${PWD}/build/x86/release
        }
    }
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    helper_p.h \
    serialport.h \
    serialport_p.h \
    serialportinfo.h \
    serialportinfo_p.h
