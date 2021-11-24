/*******************************************************************************
 * MIT License
 *
 * Copyright (c) 2021 by Lilygo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "epd2in13b.h"
#include "epdif.h"

#include <stdlib.h>

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

/******************************************************************************/
/***        local function prototypes                                       ***/
/******************************************************************************/

/******************************************************************************/
/***        exported variables                                              ***/
/******************************************************************************/

/******************************************************************************/
/***        local variables                                                 ***/
/******************************************************************************/

/******************************************************************************/
/***        exported functions                                              ***/
/******************************************************************************/

/******************************************************************************/
/***        local functions                                                 ***/
/******************************************************************************/

Epd::Epd()
{
    u8ResetPin = RST_PIN;
    u8DataCmdPin = DC_PIN;
    u8ChipSelectPin = CS_PIN;
    u8BusyPin = BUSY_PIN;
    u16MaxWidth = EPD_WIDTH;
    u16MaxHeight = EPD_HEIGHT;
}


int Epd::Init(void)
{
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) return -1;

    /* EPD hardware init start */
    Reset();
    SendCommand(BOOSTER_SOFT_START);
    SendData(0x17);
    SendData(0x17);
    SendData(0x1d);
    SendCommand(POWER_ON);
    WaitUntilIdle();
    SendCommand(PANEL_SETTING);
    SendData(0x1F);
    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x23);
    SendData(0x07);
    GateSourceStartSetting(9, 0);
    ResolutionSetting(240, 416);
    /* EPD hardware init end */
    return 0;
}


void Epd::SendCommand(uint8_t u8Command)
{
    DigitalWrite(u8DataCmdPin, LOW);
    SpiTransfer(u8Command);
}


void Epd::SendData(uint8_t u8Data)
{
    DigitalWrite(u8DataCmdPin, HIGH);
    SpiTransfer(u8Data);
}


void Epd::WaitUntilIdle(void)
{
    while(DigitalRead(u8BusyPin) == 0) //0: busy, 1: idle
    {
        DelayMs(100);
    }
}


void Epd::Reset(void)
{
    DigitalWrite(u8ResetPin, LOW);
    DelayMs(200);
    DigitalWrite(u8ResetPin, HIGH);
    DelayMs(200);
}


void Epd::ResolutionSetting(uint16_t u16Width, uint16_t u16Height)
{
    if (u16Width > EPD_WIDTH && u16Height > EPD_HEIGHT) return;

    WaitUntilIdle();
    SendCommand(RESOLUTION_SETTING);
    SendData((u16Width % 8) ? ((u16Width/8 + 1) << 0x03) & 0xFF : u16Width & 0xFF);
    SendData((u16Height >> 8) & 0x03);
    SendData(u16Height & 0xFF);
}


void Epd::GateSourceStartSetting(uint16_t x, uint16_t y)
{
    if (x > EPD_WIDTH && y > EPD_HEIGHT) return;

    WaitUntilIdle();
    SendCommand(0x65);
    SendData((x % 8) ? ((x/8 + 1) << 0x03) & 0xFF : x & 0xFF);
    SendData((y >> 8) & 0x01);
    SendData(y & 0xFF);
}


void Epd::ClearFullFrame(void)
{
    GateSourceStartSetting(0, 0);
    ResolutionSetting(240, 416);

    SendCommand(DATA_START_TRANSMISSION_1);
    DelayMs(2);
    for (int i = 0; i < this->u16MaxWidth * this->u16MaxHeight / 8; i++)
    {
        SendData(0x00);
        DelayMs(2);
    }

    SendCommand(DATA_START_TRANSMISSION_2);
    DelayMs(2);
    for (int i = 0; i < this->u16MaxWidth * this->u16MaxHeight / 8; i++)
    {
        SendData(0xFF);
    }
    DelayMs(2);

    SendCommand(DISPLAY_REFRESH);
    WaitUntilIdle();
}


void Epd::DisplayFrame(const uint8_t *u8FrameBufferBlack)
{
    GateSourceStartSetting(0, 0);
    ResolutionSetting(240, 416);

    WaitUntilIdle();
    SendCommand(DATA_START_TRANSMISSION_1);
    DelayMs(2);
    for (int i = 0; i < this->u16MaxWidth * this->u16MaxHeight / 8; i++)
    {
        SendData(0xff);
        DelayMs(2);
    }
    if (u8FrameBufferBlack != NULL)
    {
        SendCommand(DATA_START_TRANSMISSION_2);
        DelayMs(2);
        for (int i = 0; i < this->u16MaxWidth * this->u16MaxHeight / 8; i++)
        {
            SendData(u8FrameBufferBlack[i]);
        }
        DelayMs(2);
    }
    SendCommand(DISPLAY_REFRESH);
    WaitUntilIdle();
}


void Epd::LocalRefresh(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *u8FrameBufferBlack)
{
    GateSourceStartSetting(x, y);
    ResolutionSetting(w, h);
    // WaitUntilIdle();
    // SendCommand(0x65);
    // SendData(0x80);
    // SendData(0x00);
    // SendData(0x80);

    // WaitUntilIdle();
    // SendCommand(RESOLUTION_SETTING);
    // SendData(0x40);
    // SendData(0x00);
    // SendData(0x40);

    WaitUntilIdle();
    SendCommand(DATA_START_TRANSMISSION_1);
    DelayMs(2);
    for (int i = 0; i < w * h / 8; i++)
    {
        SendData(0xff);
        DelayMs(2);
    }
    if (u8FrameBufferBlack != NULL)
    {
        SendCommand(DATA_START_TRANSMISSION_2);
        DelayMs(2);
        for (int i = 0; i < w * h / 8; i++)
        {
            SendData(u8FrameBufferBlack[i]);
        }
        DelayMs(2);
    }
    SendCommand(DISPLAY_REFRESH);
    WaitUntilIdle();
}


void Epd::PowerOff(void)
{
    SendCommand(POWER_OFF);
    WaitUntilIdle();
}


void Epd::PowerOn(void)
{
    SendCommand(POWER_ON);
    WaitUntilIdle();
}


void Epd::Sleep(void)
{
    SendCommand(POWER_OFF);
    WaitUntilIdle();
    SendCommand(DEEP_SLEEP);
    SendData(0xA5);     // check code
}

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/