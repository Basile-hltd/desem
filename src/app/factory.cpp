#include "platform-config.h"
#include "board/board.h"
#include "trace/trace.h"
#ifdef USE_DISPLAY
    #include "display/simpledisplay.h"
#endif // USE_DISPLAY
#ifdef TC_MESHSIM
    #include <unistd.h>				// For getopt()
#endif // TC_MESHSIM
#include "board/factory.h"
#include "factory.h"

desenet::SlotNumber __SLOT_NUMBER = DEFAULT_DESENET_SLOT_NUMBER;        ///< Slot number variable that may be changed at startup.
#ifdef TC_MESHSIM
    QString __RESOURCE_PATH[RESOURCE_PATH_COUNT] =
                                {"",                                                // For demo program, variable needs to be empty
                                 "/../../../../src/common/platform/qt-meshsim",     // During development
                                };
#endif // TC_MESHSIM

namespace app {

Factory * Factory::_pInstance(nullptr);

Factory::Factory(int argc, char ** argv)
{
    (void)(argc); (void)(argv);

    assert(!_pInstance);
    _pInstance = this;

#ifdef TC_MESHSIM
    {
        int c;
        // Parse command line arguments...
        while ((c = getopt(argc, argv, "s:r:")) != -1)
        {
            switch(c)
            {
            case 's':
                __SLOT_NUMBER = static_cast<desenet::SlotNumber>(atoi(optarg));
                break;
            case 'r':
                __RESOURCE_PATH[0] = QString(optarg);
                break;
            }
        }
    }
#endif // TC_MESHSIM
#ifdef TC_STM32CUBEIDE
    // Update slot number according to the buttons pressed on the navigation switch
    IJoystick::PressedButtons pressedButtons = board::Joystick::instance().navigationButtons();

    pressedButtons >>= 1;       // Shift into the range of 'Left' to 'center pressed'
    if (pressedButtons != 0)    // Apply only if at least one button is pressed
    {
        __SLOT_NUMBER = pressedButtons;
    }
#endif // TC_STM32CUBEIDE
}

void Factory::buildApplication()
{
    board::initialize();	// Initialize board specific stuff

    Trace::outln("");
    Trace::outln("---------------------------------------------");
    Trace::outln("-- Starting Desenet %s                 --", DESNET_NODE_NAME);
    Trace::outln("-- Compiled: %s %s          --", __DATE__, __TIME__);
    Trace::outln("---------------------------------------------");

    //
    // Initialize objects
    //
    board::Factory::initialize();
    clockwork().initialize();

    net().initialize(__SLOT_NUMBER);
#ifdef USE_DISPLAY
    display().initialize();
#endif // USE_DISPLAY

    // Initialize applications
    accelerometerApplication().initialize();
    joystickapplication().initialize();

    //
    // Initialize relations
    //

    board::Joystick::instance().setObserver(&joystickapplication());

#ifdef USE_DISPLAY
    char str[32];
    display().clear();

    // ePaper Display on Nucleo board
    // Draw title on display
    display().drawText(DESNET_NODE_NAME, 46, 2);
    display().drawLine({5, 50}, {SimpleDisplay::X_MAX - 5, 50});
    sprintf(str, "Slot #: %d", __SLOT_NUMBER);
    display().drawText(str, 5, 10);
#endif // USE_DISPLAY

    //
    // Start threads and state-machines
    //
    board::Factory::start();
    clockwork().start();
    net().start();
    accelerometerApplication().start();
    joystickapplication().start();
}

app::AccelerometerApplication & Factory::accelerometerApplication() const
{
    static app::AccelerometerApplication accelerometerApp;

    return accelerometerApp;
}

app::Joystickapplication & Factory::joystickapplication() const
{
    static app::Joystickapplication joystickapplication;

    return joystickapplication;
}


Net & Factory::net() const
{
    static Net net;

    return net;
}

Clockwork & Factory::clockwork() const
{
    static Clockwork cw;

    return cw;
}

#ifdef USE_DISPLAY
SimpleDisplay & Factory::display()
{
    static SimpleDisplay display;

    return display;
}
#endif // USE_DISPLAY

} // namespace app

void Factory_init(int argc, char ** argv)
{
    static app::Factory factory(argc, argv);
}

void Factory_buildApplication()
{
    app::Factory::instance().buildApplication();
}
