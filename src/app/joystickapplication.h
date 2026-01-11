#ifndef JOYSTICKAPPLICATION_H
#define JOYSTICKAPPLICATION_H

#include "platform-config.h"
#include "xf/xfreactive.h"
#include "desenet/sensor/abstractapplication.h"
#include "interfaces/ijoystick.h"
#include "interfaces/ijoystickobserver.h"
#include "platform-config.h"
#include <iostream>

namespace app
{
    class Joystickapplication : public XFReactive,
                                public desenet::sensor::AbstractApplication,
                                public IJoystickObserver {

        public:
            Joystickapplication ();

            void initialize();
            void start();

            void onPositionChange( IJoystick::Position position );

    };
}
#endif // JOYSTICKAPPLICATION_H
