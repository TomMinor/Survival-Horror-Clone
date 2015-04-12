# qt 5 wants this may cause errors with 4
isEqual(QT_MAJOR_VERSION, 5) {cache() }
QT += core
QT += opengl
TARGET=SDLOpenGL
CONFIG-=app_bundle
DESTDIR=./
OBJECTS_DIR=obj
SOURCES=src/main.cpp \
        src/GLFunctions.cpp \
        src/Vec4.cpp \
        src/Mat4.cpp \
    src/World.cpp \
    src/Actor.cpp \
    src/Room.cpp \
    src/BoundingBox.cpp \
    src/Texture.cpp \
    src/Camera.cpp \
    src/Background.cpp \
    src/Door.cpp \
    src/stringUtilities.cpp \
    src/RoomReader.cpp \
    src/3dUtilities.cpp \
    src/md2loader.cpp \
    src/md2mesh.cpp \
    src/FileSystem.cpp \
    src/Inventory.cpp \
    src/Item.cpp \
    src/ItemFactory.cpp \
    src/ItemParser.cpp


HEADERS+=include/GLFunctions.h \
         include/Vec4.h \
         include/Mat4.h \
    include/World.h \
    include/Actor.h \
    include/Room.h \
    include/Door.h \
    include/BoundingBox.h \
    include/Background.h \
    include/Texture.h \
    include/Camera.h \
    include/stringUtilities.h \
    include/RoomReader.h \
    include/GLinc.h \
    include/3dUtilities.h \
    include/md2.h \
    include/md2loader.h \
    include/md2mesh.h \
    include/md2.h \
    include/FileSystem.h \
    include/Player.h \
    include/Inventory.h \
    include/Item.h \
    include/ItemFactory.h \
    include/ItemParser.h

# add the ngl lib
# this is where to look for includes
INCLUDEPATH +=include

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
