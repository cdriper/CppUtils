
TARGET = CppUtilsGithub

# Input
SOURCES += src/main.cpp \
           src/Tests/*.cpp \
           src/Tests/UtilsTests/*.cpp
           
win32 {
   SOURCES += src/Platform/Win/*.cpp
}           

macx {
   SOURCES += src/Platform/Mac/*.cpp
}           

                                
HEADERS += src/*.h \
           src/Utils/*.h
                      
INCLUDEPATH += ./src 

CONFIG += c++14 console

PRECOMPILED_HEADER = ./src/stdafx.h

DESTDIR = _build
OBJECTS_DIR = _build/obj



