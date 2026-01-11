#include "factory.h"

namespace board {

bool Factory::initialize()
{
    static bool initialized = false;

    if (!initialized)   // Initialize objects only once.
    {
        bool success = true;
        initialized = true;

        success &= accelerometer().initialize();
        success &= joystick().initialize();
        success &= ledController().initialize();

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

} // namespace board
