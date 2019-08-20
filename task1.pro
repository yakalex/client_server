TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        raw_udp.cpp \
        tcp.cpp \
        udp.cpp

HEADERS += \
    raw_udp.h \
    tcp.h \
    udp.h
