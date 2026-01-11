#ifndef BOARD_FACTORY_H
#define BOARD_FACTORY_H

#include "accel/accelerometer.h"
#include "joystick.h"
#include "ledcontroller.h"
#include "meshsimboard.h"

namespace board {

/**
 * @brief Board Factory class to provide board specific objects.
 */
class Factory
{
public:
    static bool initialize();
    static void start();

    static Accelerometer & accelerometer();
    static Joystick & joystick();
    static LedController & ledController();

    static ::MeshSimBoard & meshSimBoard();

protected:
    Factory() = default;                                ///< Prohibit public creation of board::Factory.
    Factory(const Factory &) = default;				    ///< Forbids to copy an object.
    const Factory & operator =(const Factory &);	    ///< Forbids to assign to other object.
};

} // namespace board
#endif // BOARD_FACTORY_H
