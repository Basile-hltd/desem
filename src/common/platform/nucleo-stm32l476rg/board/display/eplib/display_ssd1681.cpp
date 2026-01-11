#include <cassert>
#include "display_ssd1681.h"

namespace ep {

/******************************************************************************
function:   Perform some init instructions for the display
******************************************************************************/
void DisplaySecond::init()
{
    //GD : This annotation is used to notify that this isn't the settings that was found in Adafruit example code
    //But settings found for a better "short refresh" (most of them is certainly useless however)
    uint8_t buf[5];
    initReset();                        // Electronic paper IC reset
    HAL_Delay(100);
    lcdChkstatus();

    writeCMD(SSD1681_SW_RESET);         // soft reset
    HAL_Delay(20);
    lcdChkstatus();

    buf[0] = 0x03;
    writeCMD(SSD1681_DATA_MODE);        // Ram data entry mode
    writeDATA(buf[0]);

    buf[0] = 0x05;
    writeCMD(SSD1681_WRITE_BORDER);     // border color
    writeDATA(buf[0]);

    buf[0] = 0x80;
    writeCMD(SSD1681_TEMP_CONTROL);     // Temp control
    writeDATA(buf[0]);

    buf[0] = 0x00;
    writeCMD(SSD1681_SET_RAMXCOUNT);
    writeDATA(buf[0]);

    buf[0] = 0x00;
    buf[1] = 0x00;
    writeCMD(SSD1681_SET_RAMYCOUNT);
    writeDATA(buf[0]);
    writeDATA(buf[1]);

    // Set display size and driver output control
    buf[0] = (WIDTH - 1);
    buf[1] = (WIDTH - 1) >> 8;
    buf[2] = 0x00;
    writeCMD(SSD1681_DRIVER_CONTROL);
    writeDATA(buf[0]);
    writeDATA(buf[1]);
    writeDATA(buf[2]);

    setRamWindow(0, 0, (HEIGHT / 8) - 1, WIDTH - 1);
}

/******************************************************************************
function:   Display the array (image)
parameter:
    image :   array which contain the picture
******************************************************************************/
void DisplaySecond::display(uint8_t *image)
{
    //Set ram address to 0, 0
    setRamAddress(0, 0);

    // Transfer image
    writeCMD(SSD1681_WRITE_RAM1);
    for(uint32_t i = 0; i < 5000; i++)
    {
        writeDATA(image[i]);
    }

    update();
}

void DisplaySecond::setLUTLongRefresh()
{
    writeCMD(SSD1681_WRITE_LUT);
    for(uint32_t i = 0; i < 30; i++)
    {
        writeDATA(LUT_DATA[i]);
    }
}

void DisplaySecond::setLUTShortRefresh()
{
    writeCMD(SSD1681_WRITE_LUT);
    for(uint32_t i = 0; i < 30; i++)
    {
        writeDATA(LUT_part1[i]);
    }
}

void DisplaySecond::setRamAddress(uint16_t x, uint16_t y)
{
    uint8_t buf[2];

    // set RAM x address count
    buf[0] = x;
    writeCMD(SSD1681_SET_RAMXCOUNT);
    writeDATA(buf[0]);

    // set RAM y address count
    buf[0] = y;
    buf[1] = y >> 8;
    writeCMD(SSD1681_SET_RAMYCOUNT);
    writeDATA(buf[0]);
    writeDATA(buf[1]);
}

void DisplaySecond::setRamWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint8_t buf[4];

    // Set ram X start/end position
    buf[0] = x1;
    buf[1] = x2;
    writeCMD(SSD1681_SET_RAMXPOS);
    writeDATA(buf[0]);
    writeDATA(buf[1]);

    // Set ram Y start/end position
    buf[0] = y1;
    buf[1] = y1 >> 8;
    buf[2] = y2;
    buf[3] = y2 >> 8;
    writeCMD(SSD1681_SET_RAMYPOS);
    writeDATA(buf[0]);
    writeDATA(buf[1]);
    writeDATA(buf[2]);
    writeDATA(buf[3]);
}

void DisplaySecond::updatePartial()
{
    uint8_t buf[1];

    // display update sequence
    buf[0] = 0xFF;
    writeCMD(SSD1681_DISP_CTRL2);
    writeDATA(buf[0]);

    writeCMD(SSD1681_MASTER_ACTIVATE);
    lcdChkstatus();
}

void DisplaySecond::update()
{
    uint8_t buf[1];

    // display update sequence
    buf[0] = 0xF7;
    writeCMD(SSD1681_DISP_CTRL2);
    writeDATA(buf[0]);

    writeCMD(SSD1681_MASTER_ACTIVATE);
    lcdChkstatus();
}

} // namespace ep
