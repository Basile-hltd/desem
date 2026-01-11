#include "joystickapplication.h"

using app::Joystickapplication;

Joystickapplication::Joystickapplication () {}


void Joystickapplication::initialize(){
    //std::cout << "Joaystick init" << std::endl;

}

void Joystickapplication::start(){
    //std::cout << "Joaysti11ck start" << std::endl;
}

void Joystickapplication::onPositionChange(IJoystick::Position position){
    //std::cout << "Joaystick onPositionChange" << std::endl;

    IJoystick::PressedButtons pressedButton = position.pressedButtons;

    SharedByteBuffer buffer(1);
    buffer[0] = (pressedButton & 0x000000ff);

    evPublishRequest(EVID_JOYSTICK, buffer);
}
