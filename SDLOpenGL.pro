# qt 5 wants this may cause errors with 4
isEqual(QT_MAJOR_VERSION, 5) {cache() }
QT += core
QT += opengl
TARGET=SDLOpenGL
CONFIG-=app_bundle
DESTDIR=./
OBJECTS_DIR=obj

SOURCES=src/*.cpp \
        src/hud/*.cpp \
        src/hud/item/*.cpp \
        src/math/*.cpp \
        src/world/*.cpp

HEADERS+=include/*.h \
        include/hud/*.h \
        include/hud/item/*.h \
        include/math/*.h \
        include/world/*.h

# add the ngl lib
# this is where to look for includes
INCLUDEPATH +=include
INCLUDEPATH +=include/hud
INCLUDEPATH +=include/hud/item
INCLUDEPATH +=include/math
INCLUDEPATH +=include/world

QMAKE_CXXFLAGS += -std=c++11

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
macx:QMAKE_CXXFLAGS+= -arch x86_64
linux-g++:QMAKE_CXXFLAGS +=  -march=native
linux-g++-64:QMAKE_CXXFLAGS +=  -march=native

QMAKE_CXXFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CXXFLAGS)

LIBS+=$$system(sdl2-config  --libs)
message(output from sdl2-config --libs added to LIB=$$LIBS)

LIBS += -L/usr/local/lib
LIBS += -lSDL2_image
macx:LIBS+= -framework OpenGL
macx:DEFINES+=DARWIN
linux-*:DEFINES+=LINUX
# now if we are under unix and not on a Mac (i.e. linux) define GLEW
linux-g++:linux-g++-64 {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
linux-clang {
    DEFINES += LINUX
    LIBS+= -lGLEW
}
CONFIG+=debug_and_release
