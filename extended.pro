TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

copydata.commands = $(COPY_DIR) -r $$PWD/data $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

LIBS += -lglfw
LIBS += -lGLEW
LIBS += -lGL


SOURCES += main.cpp


HEADERS += \
    SystemAbstraction/Application/libs/nuklear/nuklear.h \
    SystemAbstraction/Application/libs/stb/stb_image.h
