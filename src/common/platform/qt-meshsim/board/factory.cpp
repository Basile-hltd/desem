#include "factory.h"

namespace board {

bool Factory::initialize()
{
    static bool initialized = false;

    if (!initialized)   // Initialize objects only once.
    {
        bool success = true;
        initialized = true;

        success &= meshSimBoard().initialize();
        success &= accelerometer().initialize();
        success &= joystick().initialize();
        success &= ledController().initialize();

        QObject::connect(&meshSimBoard(), SIGNAL(sigLastSeenMousePosition(QPoint)), &accelerometer(), SLOT(onLastSeenMousePosition(QPoint)));

        return success;
    }
    return false;
}

void Factory::start()
{
    LedController::instance().start();
}

Accelerometer & Factory::accelerometer()
{
    static Accelerometer accelerometer_;
    return accelerometer_;
}

Joystick & Factory::joystick()
{
    return Joystick::instance();
}

LedController & Factory::ledController()
{
    return LedController::instance();
}

MeshSimBoard &Factory::meshSimBoard()
{
    static MeshSimBoard msb;

    return msb;
}

} // namespace board
