#include "interfaces/ijoystickobserver.h"
#include "board/factory.h"
#include "meshsimboard.h"
#include "joystick.h"

namespace board {

Joystick::Joystick():
    _bInitialized(false)
{
}

Joystick::Joystick(const Joystick &):
    _bInitialized(false)
{
}

MeshSimBoard & Joystick::meshSimBoard() const
{
    return board::Factory::meshSimBoard();
}

bool Joystick::initialize()
{
    if (!_bInitialized)
    {
        _bInitialized = true;

        meshSimBoard().initialize();

        return true;
    }
    return false;
}

bool Joystick::setObserver(IJoystickObserver * observer)
{
    return meshSimBoard().setObserver(observer);
}

void Joystick::start()
{
    // Nothing to start here. For the simulation, the
    // MeshSimBoard instance is in charge to check and
    // notify changes to the observer

    // Note: Call the start() method anyway in the application layer!
    //       It may be needed for other Joystick implementations for
    //       other platforms!
}

//static
Joystick & Joystick::instance()
{
    static Joystick joystick;

    return joystick;
}

IJoystick::Position Joystick::position() const
{
    return meshSimBoard().position();
}

} // namespace board
