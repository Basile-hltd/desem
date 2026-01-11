#ifndef EP_DISPLAY_H
#define EP_DISPLAY_H

#include <cstdint>
#include "mcu/mcu.h"
#include "hal/gpo.h"
#include "hal/gpi.h"

class MonochromeDisplay;

namespace ep {
/**
 * @class Display
 * @brief Base class for the display
 * Will provide support for either a SSD1608 or a SSD1681 chipset display.
 */
class Display
{
    friend class ::MonochromeDisplay;
public:
    Display();
    virtual ~Display() = default;
    void initialize(SPI_HandleTypeDef * hspi2, Gpo * pinDC, Gpi * pinBusy, Gpo * pinReset);
    static Display * instance();

    // Abstract methods to be implemented by the inheriting class
    virtual void init() =0;
    virtual void display(uint8_t * image) =0;
    virtual void setLUTLongRefresh() =0;
    virtual void setLUTShortRefresh() =0;

protected:
    void writeCMD(uint8_t command);
    void writeDATA(uint8_t command);
    void SPI_Write(unsigned char command);
    void initReset();
    void lcdChkstatus();

protected:
    static Display * pInstance;

    SPI_HandleTypeDef * spi;    ///< SPI used for communication.
    Gpo * pinDC;                ///< Data command pin for display.
    Gpi * pinBusy;              ///< Busy pin which indicates if display is free or not.
    Gpo * pinReset;             ///< Reset pin for the display.
};

} // namespace ep
#endif // EP_DISPLAY_H
