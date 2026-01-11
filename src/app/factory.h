#ifndef FACTORY_H
#define FACTORY_H

//
// What is seen only by the C++ compiler
//
#ifdef __cplusplus

#include <cassert>
#include "platform-config.h"
#include "desenet/sensor/net.h"
#include "board/interfaces/imonochromedisplay.h"
#include "app/accelerometerapplication.h"
#include "board/accel/accelerometer.h"
#include "app/joystickapplication.h"
#include "clockwork/clockwork.h"

using desenet::sensor::Net;

#ifdef TC_MESHSIM
	class MeshSimBoard;		// Class making the Olimex board view of the simulator

	extern desenet::SlotNumber __SLOT_NUMBER;
	const constexpr uint32_t RESOURCE_PATH_COUNT = 2;
	extern QString __RESOURCE_PATH[RESOURCE_PATH_COUNT];
#endif // TC_MESHSIM
class SimpleDisplay;

namespace app {

/**
 * @brief Provides access to general instances. Initializes and builds the application.
 */
class Factory
{
public:
	Factory(int argc, char ** argv);

	void buildApplication();

	static Factory & instance() { assert(_pInstance); return *_pInstance; }

    app::AccelerometerApplication & accelerometerApplication() const;
    app::Joystickapplication & joystickapplication() const;

	Net & net() const;
	Clockwork & clockwork() const;

#ifdef USE_DISPLAY
	SimpleDisplay & display();
#endif // USE_DISPLAY

protected:
	static Factory * _pInstance;
};

} // namespace app
#endif // __cplusplus

//
// What is seen by the C and C++ compiler
//
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void Factory_init(int argc, char ** argv);      ///< C-Wrapper to Factory::init() method
void Factory_buildApplication();                ///< C-Wrapper to Factory::buildApplication() method

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FACTORY_H
