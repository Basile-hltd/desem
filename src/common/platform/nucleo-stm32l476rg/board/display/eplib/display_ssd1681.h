#ifndef EP_DISPLAY_SSD1681_H
#define EP_DISPLAY_SSD1681_H

#include <cstdint>
#include "mcu/mcu.h"
#include "lutarray.h"
#include "display.h"

#define WIDTH                       200
#define HEIGHT                      200
#define TOTAL_BYTES                 5000

#define SSD1681_DRIVER_CONTROL 0x01
#define SSD1681_GATE_VOLTAGE 0x03
#define SSD1681_SOURCE_VOLTAGE 0x04
#define SSD1681_PROGOTP_INITIAL 0x08
#define SSD1681_PROGREG_INITIAL 0x09
#define SSD1681_READREG_INITIAL 0x0A
#define SSD1681_BOOST_SOFTSTART 0x0C
#define SSD1681_DEEP_SLEEP 0x10
#define SSD1681_DATA_MODE 0x11
#define SSD1681_SW_RESET 0x12
#define SSD1681_TEMP_CONTROL 0x18
#define SSD1681_TEMP_WRITE 0x1A
#define SSD1681_MASTER_ACTIVATE 0x20
#define SSD1681_DISP_CTRL1 0x21
#define SSD1681_DISP_CTRL2 0x22
#define SSD1681_WRITE_RAM1 0x24
#define SSD1681_WRITE_RAM2 0x26
#define SSD1681_WRITE_VCOM 0x2C
#define SSD1681_READ_OTP 0x2D
#define SSD1681_READ_STATUS 0x2F
#define SSD1681_WRITE_LUT 0x32
#define SSD1681_WRITE_BORDER 0x3C
#define SSD1681_SET_RAMXPOS 0x44
#define SSD1681_SET_RAMYPOS 0x45
#define SSD1681_SET_RAMXCOUNT 0x4E
#define SSD1681_SET_RAMYCOUNT 0x4F

namespace ep {
/**
 * @class Display
 * @brief This class is used to perform the communication between the uP and the chip controlling the display (here SSD1681), the communication is performed with the SPI protocol
 * For that, we need to write to the RAM of the chip the value of each pixel (0 ->black, 1 ->white), to draw something special, like a line or a circle, the paint class will help you, here
 * we only take an array containing the value of each pixel.
 */
class DisplaySecond : public Display
{
public:
    DisplaySecond() = default;
    virtual ~DisplaySecond() = default;

    void init() override;
    void display(uint8_t * image) override;
    void setLUTLongRefresh() override;
    void setLUTShortRefresh() override;

protected:
    void setRamAddress(uint16_t x, uint16_t y);
    void setRamWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void updatePartial();
    void update();
};

} // namespace ep

#endif // EP_DISPLAY_SSD1681_H
