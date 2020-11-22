TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
LIBS   += -lepoxy
LIBS   += -lGL
LIBS   += -lglfw

SOURCES += \
    Wojciech_Strak_03.cpp \
    AGL3Window.cpp \
    myline.cpp \
    myboard.cpp \
    backgroundrectangle.cpp \
    camera.cpp \
    myshape.cpp
HEADERS += \
    AGL3Window.hpp \
    AGL3Drawable.hpp \
    myline.h \
    myboard.h \
    backgroundrectangle.h \
    camera.h \
    myshape.h \
    myShape.vs \
    myShape.fs \
    triangle.h
