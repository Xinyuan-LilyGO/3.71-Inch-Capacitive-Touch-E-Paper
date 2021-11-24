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

#ifndef EPD2IN13B_H
#define EPD2IN13B_H

/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "epdif.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

/**
 * @brief Display resolution
 */
#define EPD_WIDTH       240
#define EPD_HEIGHT      416

/**
 * @brief EPD2IN13B commands
 */
#define PANEL_SETTING                               0x00
#define POWER_SETTING                               0x01
#define POWER_OFF                                   0x02
#define POWER_OFF_SEQUENCE_SETTING                  0x03
#define POWER_ON                                    0x04
#define POWER_ON_MEASURE                            0x05
#define BOOSTER_SOFT_START                          0x06
#define DEEP_SLEEP                                  0x07
#define DATA_START_TRANSMISSION_1                   0x10
#define DATA_STOP                                   0x11
#define DISPLAY_REFRESH                             0x12
#define DATA_START_TRANSMISSION_2                   0x13
#define VCOM_LUT                                    0x20
#define W2W_LUT                                     0x21
#define B2W_LUT                                     0x22
#define W2B_LUT                                     0x23
#define B2B_LUT                                     0x24
#define PLL_CONTROL                                 0x30
#define TEMPERATURE_SENSOR_CALIBRATION              0x40
#define TEMPERATURE_SENSOR_SELECTION                0x41
#define TEMPERATURE_SENSOR_WRITE                    0x42
#define TEMPERATURE_SENSOR_READ                     0x43
#define VCOM_AND_DATA_INTERVAL_SETTING              0x50
#define LOW_POWER_DETECTION                         0x51
#define TCON_SETTING                                0x60
#define RESOLUTION_SETTING                          0x61
#define GET_STATUS                                  0x71
#define AUTO_MEASURE_VCOM                           0x80
#define READ_VCOM_VALUE                             0x81
#define VCM_DC_SETTING                              0x82
#define PARTIAL_WINDOW                              0x90
#define PARTIAL_IN                                  0x91
#define PARTIAL_OUT                                 0x92
#define PROGRAM_MODE                                0xA0
#define ACTIVE_PROGRAM                              0xA1
#define READ_OTP_DATA                               0xA2
#define POWER_SAVING                                0xE3

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

class Epd : EpdIf
{
public:
    uint16_t u16MaxWidth;
    uint16_t u16MaxHeight;
    uint16_t u16CurX;
    uint16_t u16CurY;
    uint16_t u16CurWidth;
    uint16_t u16CurHeight;

    Epd();
    ~Epd() { };
    int32_t Init(void);

    /**
     * @brief basic function for sending commands
     *
     * @param[in] u8Command command id
     */
    void SendCommand(uint8_t u8Command);

    /**
     * @brief basic function for sending data
     *
     * @param[in] u8Data valid data
     */
    void SendData(uint8_t u8Data);

    /**
     * @brief Wait until the u8BusyPin goes HIGH
     */
    void WaitUntilIdle(void);

    /**
     * @brief module reset.
     *
     * @note often used to awaken the module in deep sleep, @see Epd::Sleep();
     */
    void Reset(void);

    /**
     * @brief  start gate/source position.
     */
    void GateSourceStartSetting(uint16_t x, uint16_t y);

    /**
     * @brief resolution setting.
     */
    void ResolutionSetting(uint16_t u16Width, uint16_t u16Height);

    /**
     * @brief Clear EPD.
     */
    void ClearFullFrame(void);

    /**
     * @brief display the frame buffers
     *
     * @param[in] u8FrameBufferBlack frame buffer
     */
    void DisplayFrame(const uint8_t *u8FrameBufferBlack);

    /**
     * @brief Displays content in the specified area.
     */
    void LocalRefresh(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *u8FrameBufferBlack);

    /**
     * @brief After this command is transmitted, the chip would enter the
     *        deep-sleep mode to save power.
     *
     * @note The deep sleep mode would return to standby by hardware reset.
     *       The only one parameter is a check code, the command would be
     *       executed if check code = 0xA5.
     *       You can use @see Epd::Init() to awaken
     */
    void Sleep(void);

    void PowerOff(void);

    void PowerOn(void);

private:
    uint8_t u8ResetPin;
    uint8_t u8DataCmdPin;
    uint8_t u8ChipSelectPin;
    uint8_t u8BusyPin;
};

/******************************************************************************/
/***        exported variables                                              ***/
/******************************************************************************/

/******************************************************************************/
/***        exported functions                                              ***/
/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* EPD2IN13B_H */
/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/