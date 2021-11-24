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

#include "epdpaint.h"

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

Paint::Paint(uint8_t *image, uint32_t width, uint32_t height)
{
    this->rotate = ROTATE_0;
    this->image = image;
    /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
    // this->width = width % 8 ? width + 8 - (width % 8) : width;
    this->width = width & (8 - 1) ? width + 8 - (width & (8 - 1)) : width;
    this->height = height;
}


void Paint::Clear(int32_t colored)
{
    for (int32_t x = 0; x < this->width; x++)
    {
        for (int32_t y = 0; y < this->height; y++)
        {
            DrawAbsolutePixel(x, y, colored);
        }
    }
}


void Paint::DrawAbsolutePixel(int32_t x, int32_t y, int32_t colored)
{
    if (x < 0 || x >= this->width || y < 0 || y >= this->height)
    {
        return;
    }

    if (IF_INVERT_COLOR)
    {
        if (colored)
        {
            image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
        }
        else
        {
            image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
        }
    }
    else
    {
        if (colored)
        {
            image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
        }
        else
        {
            image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
        }
    }
}


uint8_t *Paint::GetImage(void)
{
    return this->image;
}


int32_t Paint::GetWidth(void)
{
    return this->width;
}


void Paint::SetWidth(int32_t width)
{
    this->width = width % 8 ? width + 8 - (width % 8) : width;
}


int32_t Paint::GetHeight(void)
{
    return this->height;
}


void Paint::SetHeight(int32_t height)
{
    this->height = height;
}


int32_t Paint::GetRotate(void)
{
    return this->rotate;
}


void Paint::SetRotate(int32_t rotate)
{
    this->rotate = rotate;
}


void Paint::DrawPixel(int32_t x, int32_t y, int32_t colored)
{
    int32_t point_temp;
    if (this->rotate == ROTATE_0)
    {
        if (x < 0 || x >= this->width || y < 0 || y >= this->height)
        {
            return;
        }
        DrawAbsolutePixel(x, y, colored);
    }
    else if (this->rotate == ROTATE_90)
    {
        if (x < 0 || x >= this->height || y < 0 || y >= this->width)
        {
            return;
        }
        point_temp = x;
        x = this->width - y;
        y = point_temp;
        DrawAbsolutePixel(x, y, colored);
    }
    else if (this->rotate == ROTATE_180)
    {
        if (x < 0 || x >= this->width || y < 0 || y >= this->height)
        {
            return;
        }
        x = this->width - x;
        y = this->height - y;
        DrawAbsolutePixel(x, y, colored);
    }
    else if (this->rotate == ROTATE_270)
    {
        if (x < 0 || x >= this->height || y < 0 || y >= this->width)
        {
            return;
        }
        point_temp = x;
        x = y;
        y = this->height - point_temp;
        DrawAbsolutePixel(x, y, colored);
    }
}


void Paint::DrawCharAt(int32_t x, int32_t y, char ascii_char, sFONT *font, int32_t colored)
{
    int32_t i, j;
    uint32_t char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const uint8_t *ptr = &font->table[char_offset];

    for (j = 0; j < font->Height; j++)
    {
        for (i = 0; i < font->Width; i++)
        {
            if (*ptr & (0x80 >> (i % 8)))
            {
                DrawPixel(x + i, y + j, colored);
            }
            if (i % 8 == 7)
            {
                ptr++;
            }
        }
        if (font->Width % 8 != 0)
        {
            ptr++;
        }
    }
}


void Paint::DrawStringAt(int32_t x, int32_t y, const char *text, sFONT *font, int32_t colored)
{
    const char *p_text = text;
    uint32_t counter = 0;
    int32_t refcolumn = x;
    int32_t refrow = y;

    /* Send the string character by character on EPD */
    while (*p_text != 0)
    {
        if (refcolumn + font->Width > 241)
        {
            refrow += 8 + 2;
            refcolumn = x;
        }
        /* Display one character on EPD */
        DrawCharAt(refcolumn, refrow, *p_text, font, colored);
        /* Decrement the column position by 16 */
        refcolumn += font->Width;
        /* Point on the next character */
        p_text++;
        counter++;
    }
}


void Paint::DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t colored)
{
    /* Bresenham algorithm */
    int32_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t err = dx + dy;

    while ((x0 != x1) && (y0 != y1))
    {
        DrawPixel(x0, y0, colored);
        if (2 * err >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (2 * err <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}


void Paint::DrawHorizontalLine(int32_t x, int32_t y, int32_t line_width, int32_t colored)
{
    int32_t i;
    for (i = x; i < x + line_width; i++)
    {
        DrawPixel(i, y, colored);
    }
}


void Paint::DrawVerticalLine(int32_t x, int32_t y, int32_t line_height, int32_t colored)
{
    int32_t i;
    for (i = y; i < y + line_height; i++)
    {
        DrawPixel(x, i, colored);
    }
}


void Paint::DrawRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t colored)
{
    int32_t min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    DrawHorizontalLine(min_x, min_y, max_x - min_x + 1, colored);
    DrawHorizontalLine(min_x, max_y, max_x - min_x + 1, colored);
    DrawVerticalLine(min_x, min_y, max_y - min_y + 1, colored);
    DrawVerticalLine(max_x, min_y, max_y - min_y + 1, colored);
}


void Paint::DrawFilledRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t colored)
{
    int32_t min_x, min_y, max_x, max_y;
    int32_t i;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    for (i = min_x; i <= max_x; i++)
    {
        DrawVerticalLine(i, min_y, max_y - min_y + 1, colored);
    }
}


void Paint::DrawCircle(int32_t x, int32_t y, int32_t radius, int32_t colored)
{
    /* Bresenham algorithm */
    int32_t x_pos = -radius;
    int32_t y_pos = 0;
    int32_t err = 2 - 2 * radius;
    int32_t e2;

    do
    {
        DrawPixel(x - x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y - y_pos, colored);
        DrawPixel(x - x_pos, y - y_pos, colored);
        e2 = err;
        if (e2 <= y_pos)
        {
            err += ++y_pos * 2 + 1;
            if (-x_pos == y_pos && e2 <= x_pos)
            {
                e2 = 0;
            }
        }
        if (e2 > x_pos)
        {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}


void Paint::DrawFilledCircle(int32_t x, int32_t y, int32_t radius, int32_t colored)
{
    /* Bresenham algorithm */
    int32_t x_pos = -radius;
    int32_t y_pos = 0;
    int32_t err = 2 - 2 * radius;
    int32_t e2;

    do
    {
        DrawPixel(x - x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y - y_pos, colored);
        DrawPixel(x - x_pos, y - y_pos, colored);
        DrawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, colored);
        DrawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, colored);
        e2 = err;
        if (e2 <= y_pos)
        {
            err += ++y_pos * 2 + 1;
            if (-x_pos == y_pos && e2 <= x_pos)
            {
                e2 = 0;
            }
        }
        if (e2 > x_pos)
        {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/