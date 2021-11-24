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

#include "epdif.h"

#include <bcm2835.h>

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

void EpdIf::DigitalWrite(uint8_t u8Pin, uint8_t u8Value)
{
    bcm2835_gpio_write(u8Pin, u8Value);
}


int EpdIf::DigitalRead(uint8_t u8Pin)
{
    return bcm2835_gpio_lev(u8Pin);
}


void EpdIf::DelayMs(uint32_t u32DelayMs)
{
    bcm2835_delay(u32DelayMs);
}


void EpdIf::SpiTransfer(uint8_t u8Data)
{
    bcm2835_spi_transfer(u8Data);
}


int EpdIf::IfInit(void)
{
    if(!bcm2835_init()) return -1;

    /** Start spi interface, set spi pin for the reuse function */
    bcm2835_spi_begin();
    /** High first transmission */
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    /** spi mode 0 */
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    /** Frequency */
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);
    /** set CE0 */
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    /** enable cs0 */
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

    bcm2835_gpio_fsel(RST_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(BUSY_PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_fsel(DC_PIN, BCM2835_GPIO_FSEL_OUTP);

    return 0;
}

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/