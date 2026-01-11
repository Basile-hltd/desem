PRI_PATH = $$PWD

BOARD_PATH = "$${PRI_PATH}"

INCLUDEPATH += \
    "$${BOARD_PATH}/.." \
    "$${BOARD_PATH}"

HEADERS += \
    $$PWD/accel/accelerometer.h \
    $$PWD/board.h \
    $$PWD/button.h \
    $$PWD/display/simpledisplay.h \
    $$PWD/factory.h \
    $$PWD/joystick.h \
    $$PWD/led.h \
    $$PWD/ledcontroller.h \
    $$PWD/meshsimboard.h \
    $$PWD/navigationjoystick.h

SOURCES += \
    $$PWD/accel/accelerometer.cpp \
    $$PWD/board.cpp \
    $$PWD/button.cpp \
    $$PWD/display/simpledisplay.cpp \
    $$PWD/factory.cpp \
    $$PWD/joystick.cpp \
    $$PWD/led.cpp \
    $$PWD/ledcontroller.cpp \
    $$PWD/meshsimboard.cpp \
    $$PWD/navigationjoystick.cpp \
    $$PWD/trace.cpp
