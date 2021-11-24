
#include "epd2in13b.h"
#include "epdpaint.h"
#include "button.h"
#include "fonts.h"

#include <bcm2835.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>


#define COLORED      1
#define UNCOLORED    0

const char *str = "LilyGo";
const char *str1 = "ePaper Demo";
const char *str2 = "An individual human existence should be like a river—small at first, narrowly contained within its banks, and rushing passionately past boulders and over waterfalls. Gradually the river grows wider, the banks recede, the waters flow more quietly, and in the end, without any visible break, they become merged in the sea, and painlessly lose their individual being.";
const char *str3 = "Youth means a temperamental predominance of courage over timidity,of the appetite for adventure over the love of ease.This often exists in a man of sixty more than a boy of twenty.Nobody grows old merely by a number of years.We grow old by deserting our ideals.";
const char *str4 = "Whether sixty or sixteen, there is in every human being's heart the lure of wonders, the unfailing childlike appetite of what's next and the joy of the game of living. In the center of your heart and my heart there is a wireless station: so long as it receives messages of beauty, hope, cheer, courage and power from men and from the infinite, so long are you young.";

uint8_t cur_index = 0;

void drawNext(Epd *epd, Paint *paint_black,const char *str)
{
    int16_t x1, y1;
    uint16_t w, h;

    paint_black->Clear(UNCOLORED);
    paint_black->DrawStringAt(20, 20, str, &Font12, COLORED);
    epd->DisplayFrame(paint_black->GetImage());
}


//Tips//
/**
 * When the electronic paper is refreshed in full screen, the picture flicker is
 * a normal phenomenon, and the main function is to clear the display afterimage
 * in the previous picture.
 * When the local refresh is performed, the screen does not flash.
 */
/**
 * When you need to transplant the driver, you only need to change the
 * corresponding IO. The BUSY pin is the input mode and the others are the
 * output mode.
 */

void btn1_click_cb()
{
    printf("btn1 is click!!!\n");
    cur_index++; cur_index %= 3;
}
void btn2_click_cb()
{
    printf("btn2 is click!!!\n");
    cur_index++; cur_index %= 3;
}
void btn3_click_cb()
{
    printf("btn3 is click!!!\n");
    cur_index++; cur_index %= 3;
}

Button *btn1 = nullptr;
Button *btn2 = nullptr;
Button *btn3 = nullptr;

int main(void)
{
    Epd epd;

    // paint_black.Clear(UNCOLORED);
    if (epd.Init() != 0)
    {
        printf("e-Paper init failed\n");
        return -1;
    }

    unsigned char* frame_black = (unsigned char*)malloc(epd.u16MaxWidth * epd.u16MaxHeight / 8);
    Paint paint_black(frame_black, epd.u16MaxWidth, epd.u16MaxHeight);
    paint_black.Clear(UNCOLORED);

    btn1 = new Button(17, BCM2835_GPIO_PUD_UP, DEBOUNCE_MS);
    btn2 = new Button(27, BCM2835_GPIO_PUD_UP, DEBOUNCE_MS);
    btn3 = new Button(22, BCM2835_GPIO_PUD_UP, DEBOUNCE_MS);

    // /* Display the image buffer */
    drawNext(&epd, &paint_black, str1);
    // epd.PowerOff(); //EPD_sleep,Sleep instruction is necessary, please do not delete!!!

    btn1->setClickHandler(btn1_click_cb);
    btn2->setClickHandler(btn2_click_cb);
    btn3->setClickHandler(btn3_click_cb);

    btn1->start();
    btn2->start();
    btn3->start();

    uint8_t prev_index = 0;
    while(1)
    {
        if (prev_index == cur_index) {usleep(10 * 1000); continue;}
        prev_index = cur_index;
        switch (cur_index) {
            case 0:
                drawNext(&epd, &paint_black, str2);
            break;
            case 1:
                drawNext(&epd, &paint_black, str3);
            break;
            case 2:
                drawNext(&epd, &paint_black, str4);
            break;
            default: break;
        }
        usleep(10 * 1000);
    }
    epd.Sleep();
    return 0;
}

