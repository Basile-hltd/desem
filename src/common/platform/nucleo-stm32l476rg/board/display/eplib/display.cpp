#include <cassert>
#include "display.h"

namespace ep {

Display * Display::pInstance = nullptr;

Display::Display()
{
    spi = nullptr;
    pinDC = nullptr;
    pinBusy = nullptr;
    pinReset = nullptr;
    pInstance = nullptr;
}

/******************************************************************************
function:	initialize pointers and send a first configuration to the display
parameter:
    spi      :   device spi used to communicate with the display
    pinDC    :   pin used to say to the display if it's some Data or a command
    pinBusy  :   pin used by the display to say if busy or not
    pinReset :   pin used to perform a reset to the display
******************************************************************************/
void Display::initialize(SPI_HandleTypeDef * spi, Gpo * pinDC, Gpi * pinBusy, Gpo * pinReset)
{
    if (spi && pinDC && pinBusy && pinReset)
    {
        this->spi = spi;
        this->pinDC = pinDC;
        this->pinBusy = pinBusy;
        this->pinReset = pinReset;
        pInstance = this;
    }
    else
    {
        assert(false);
    }
    init();
}

Display * Display::instance()
{
    return pInstance;
}

/******************************************************************************
function:	communicate with the command to the display
parameter:
    command :   value of the data or the command
******************************************************************************/
void Display::SPI_Write(unsigned char command)
{
    HAL_SPI_Transmit(this->spi, &command, 1, -1);
}

/******************************************************************************
function:	write a command to the display (well, to the chip)
parameter:
    command :   value of the command
******************************************************************************/
void Display::writeCMD(uint8_t command)
{
    this->pinDC->off();
    SPI_Write(command);
}

/******************************************************************************
function:	write some data to the display (well, to the chip)
parameter:
    command : value of the data
******************************************************************************/
void Display::writeDATA(uint8_t command)
{
    this->pinDC->on();
    SPI_Write(command);
}

/******************************************************************************
function:	Perform a reset to the display
******************************************************************************/
void Display::initReset()
{
    this->pinReset->off();
    HAL_Delay(50);
    this->pinReset->on();
    HAL_Delay(50);
}

/******************************************************************************
function:	Check if the display is busy
******************************************************************************/
void Display::lcdChkstatus()
{
    GPIO_PinState busy = this->pinBusy->read();

    while (busy == GPIO_PIN_SET)
    {
        HAL_Delay(10);
        busy = this->pinBusy->read();
    }
}

} // namespace ep
