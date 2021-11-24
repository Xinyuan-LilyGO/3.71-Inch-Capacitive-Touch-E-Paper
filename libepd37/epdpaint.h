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

#ifndef EPDPAINT_H
#define EPDPAINT_H

/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "fonts.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

/**
 * @brief Display orientation
 */
#define ROTATE_0      0
#define ROTATE_90     1
#define ROTATE_180    2
#define ROTATE_270    3

/**
 * @brief Color inverse.
 *
 * @note 1 or 0 = set or reset a bit if set a colored pixel
 */
#define IF_INVERT_COLOR 0

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

class Paint
{
public:
    Paint(uint8_t *image, uint32_t width, uint32_t height);
    ~Paint() { };

    /**
     * @brief clear the image.
     *
     * @param[in] colored color
     */
    void Clear(int32_t colored);

    /**
     * @brief this draws a pixel by absolute coordinates.
     *
     * @note this function won't be affected by the rotate parameter.
     */
    int32_t GetWidth(void);

    /**
     * @brief
     *
     * @param[in] width
     */
    void SetWidth(int32_t width);

    /**
     * @brief
     *
     */
    int32_t GetHeight(void);

    /**
     * @brief
     *
     * @param[in] height
     */
    void SetHeight(int32_t height);

    /**
     * @brief
     */
    int32_t GetRotate(void);

    /**
     * @brief
     *
     * @param[in] rotate
     */
    void SetRotate(int32_t rotate);

    /**
     * @brief Getters and Setters.
     */
    uint8_t *GetImage(void);

    /**
     * @brief
     *
     * @param[in] x
     * @param[in] y
     * @param[in] colored
     */
    void DrawAbsolutePixel(int32_t x, int32_t y, int32_t colored);

    /**
     * @brief this draws a pixel by the coordinates.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] colored
     */
    void DrawPixel(int32_t x, int32_t y, int32_t colored);

    /**
     * @brief this draws a charactor on the frame buffer but not refresh.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] ascii_char
     * @param[in] font
     * @param[in] colored
     */
    void DrawCharAt(int32_t x, int32_t y, char ascii_char, sFONT *font, int32_t colored);

    /**
     * @brief this displays a string on the frame buffer but not refresh.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] text
     * @param[in] font
     * @param[in] colored
     */
    void DrawStringAt(int32_t x, int32_t y, const char *text, sFONT *font, int32_t colored);

    /**
     * @brief this draws a line on the frame buffer.
     *
     * @param[in] x0
     * @param[in] y0
     * @param[in] x1
     * @param[in] y1
     * @param[in] colored
     */
    void DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t colored);

    /**
     * @brief this draws a horizontal line on the frame buffer.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] width
     * @param[in] colored
     */
    void DrawHorizontalLine(int32_t x, int32_t y, int32_t width, int32_t colored);

    /**
     * @brief this draws a vertical line on the frame buffer.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] height
     * @param[in] colored
     */
    void DrawVerticalLine(int32_t x, int32_t y, int32_t height, int32_t colored);

    /**
     * @brief this draws a rectangle.
     *
     * @param[in] x0
     * @param[in] y0
     * @param[in] x1
     * @param[in] y1
     * @param[in] colored
     */
    void DrawRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t colored);

    /**
     * @brief this draws a filled rectangle.
     *
     * @param[in] x0
     * @param[in] y0
     * @param[in] x1
     * @param[in] y1
     * @param[in] colored
     */
    void DrawFilledRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t colored);

    /**
     * @brief this draws a circle.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] radius
     * @param[in] colored
     */
    void DrawCircle(int32_t x, int32_t y, int32_t radius, int32_t colored);

    /**
     * @brief this draws a filled circle.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] radius
     * @param[in] colored
     */
    void DrawFilledCircle(int32_t x, int32_t y, int32_t radius, int32_t colored);

private:
    uint8_t *image;
    int32_t width;
    int32_t height;
    int32_t rotate;
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