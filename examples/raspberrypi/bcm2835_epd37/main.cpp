
#include "epd2in13b.h"
#include "imagedata.h"
#include "epdpaint.h"

#include <bcm2835.h>

#include <stdlib.h>
#include <stdio.h>


#define COLORED      1
#define UNCOLORED    0

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

int main(void)
{
    Epd epd;
    if (epd.Init() != 0)
    {
        printf("e-Paper init failed\n");
        return -1;
    }

    epd.ClearFullFrame();

    unsigned char* frame_black = (unsigned char*)malloc(64 * 64 / 8);
    Paint paint_black(frame_black, 64, 64);

    paint_black.Clear(UNCOLORED);
    paint_black.DrawStringAt(0, 0, "LILYGO", &Font12, COLORED);
    epd.LocalRefresh(64, 64, 64, 64, frame_black);
    bcm2835_delay(2000);

    /* Display the image buffer */
    epd.DisplayFrame(gImage_logo);

    epd.Sleep(); //EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    // while(1);
    return 0;
}

