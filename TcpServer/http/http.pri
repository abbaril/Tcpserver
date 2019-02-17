QT += network
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include(../tcp/tcp.pri)

HEADERS += \
    $$PWD/httpserver.h \
    $$PWD/httpconnection.h \
    $$PWD/ratetransfer.h

SOURCES += \
    $$PWD/httpserver.cpp \
    $$PWD/httpconnection.cpp \
    $$PWD/ratetransfer.cpp
