
#include "epd2in13b.h"
#include "epdpaint.h"
#include "fonts.h"
#include "FT6336.h"

#include <bcm2835.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>

#define COLORED      1
#define UNCOLORED    0

void drawNext(Epd *epd, Paint *paint_black, const char *str)
{
    int16_t x1, y1;
    uint16_t w, h;

    paint_black->Clear(UNCOLORED);
    paint_black->DrawStringAt(20, 20, str, &Font12, COLORED);
    epd->DisplayFrame(paint_black->GetImage());
}

uint16_t u16RandX;
uint16_t u16RandY;

void drawButton(Epd *epd, Paint *paint_black, const char *str)
{
    u16RandX = rand()%(epd->u16MaxWidth);
    u16RandY = rand()%(epd->u16MaxWidth);
    printf("u16RandX: %d, u16RandY: %d\n", u16RandX, u16RandY);
    paint_black->Clear(UNCOLORED);
    paint_black->DrawRectangle(u16RandX, u16RandY, u16RandX + 60, u16RandY + 20, COLORED);
    paint_black->DrawStringAt(u16RandX+4, u16RandY + 4, str, &Font12, COLORED);
    epd->DisplayFrame(paint_black->GetImage());
}

uint16_t tp_x = 0;
uint16_t tp_y = 0;
void tp_click_cb(uint16_t x, uint16_t y)
{
    tp_x = x;
    tp_y = y;
    printf("x1: %d, y1: %d\n", x, y);
}

int main(void)
{
    Epd epd;
    if (epd.Init() != 0)
    {
        printf("e-Paper init failed\n");
        return -1;
    }
    FT6336 tp;
    tp.setPressedHandler(tp_click_cb);
    unsigned char* frame_black = (unsigned char*)malloc(epd.u16MaxWidth * epd.u16MaxHeight / 8);
    Paint paint_black(frame_black, epd.u16MaxWidth, epd.u16MaxHeight);

    drawButton(&epd, &paint_black, "Click Me");

    while(1)
    {
        if ((tp_x >= u16RandX && tp_x <= (u16RandX + 60)) && \
            (tp_y >= u16RandY && tp_y <= (u16RandY + 20)))
        {
            printf("draw\n");
            drawButton(&epd, &paint_black, "Click Me");
        }
        usleep(10 * 1000);
    }
    epd.Sleep();
    return 0;
}

