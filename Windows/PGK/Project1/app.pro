TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -pthread -Wall -pedantic
LIBS   += -lepoxy
LIBS   += -lGL
LIBS   += -lglfw


SOURCES += \
    main.cpp \
    openglparticlerenderer.cpp \
    particledatacontainer.cpp \
    particleemitter.cpp \
    multithreadoverseer.cpp \
    particlegenerators.cpp \
    particlesystem.cpp \
    particleupdaters.cpp \
    AGL3Window.cpp \
    camera.cpp

HEADERS += \
    InterfaceParticleRenderer.h \
    openglparticlerenderer.h \
    particledatacontainer.h \
    particleemitter.h \
    particlegenerator.h \
    multithreadoverseer.h \
    particlegenerators.h \
    particlesystem.h \
    particleupdater.h \
    particleupdaters.h \
    shader.h \
    AGL3Window.hpp \
    camera.h

DISTFILES += \
    simpleshader.frag \
    simpleshader.vert
