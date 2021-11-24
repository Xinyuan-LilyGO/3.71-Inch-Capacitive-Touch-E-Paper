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

#ifndef EPDIF_H
#define EPDIF_H

/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

/**
 * @brief Pin definition
 */
#define RST_PIN         4
#define DC_PIN          9
#define CS_PIN          8
#define BUSY_PIN        25

/**
 * @brief Pin level definition
 */
#define LOW             0
#define HIGH            1

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

class EpdIf
{
public:
    EpdIf(void) { };
    ~EpdIf(void) { };

    /**
     * @brief Initialize spi and pin.
     *
     * @return
     *    - 0 on success
     *    - -1 failed
     */
    static int32_t IfInit(void);

    /**
     * @brief Sets the specified pin output to HIGH.
     *
     * @param[in] u8Pin   GPIO number
     * @param[in] u8Value HIGH sets the output to HIGH and LOW to LOW.
     */
    static void DigitalWrite(uint8_t u8Pin, uint8_t u8Value);

    /**
     * @brief Reads the current level on the specified pin and returns either
     *        HIGH or LOW. Works whether or not the pin is an input or an output.
     *
     * @param[in] u8Pin GPIO number
     *
     * @return
     *    - 0 Low
     *    - 1 High
     */
    static int32_t DigitalRead(uint8_t u8Pin);

    /**
     * @brief Delays for the specified number of milliseconds.
     *
     * @param[in] u32DelayMs Delay in milliseconds
     */
    static void DelayMs(uint32_t u32DelayMs);

    /**
     * @brief Transfers one byte to and from the currently selected SPI slave.
     *
     * @param[in] u8Data The 8 bit data byte to write to MOSI
     */
    static void SpiTransfer(uint8_t u8Data);
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

#endif
/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/