PRI_PATH = $$PWD

CLOCKWORK_SRC_PATH = "$${PRI_PATH}"

INCLUDEPATH += \
    "$${CLOCKWORK_SRC_PATH}/.."

HEADERS += \
    $${CLOCKWORK_SRC_PATH}/sharedbuffer.hpp \
    $${CLOCKWORK_SRC_PATH}/SharedBuffer\
    $${CLOCKWORK_SRC_PATH}/SharedByteBuffer

# SOURCES += 
