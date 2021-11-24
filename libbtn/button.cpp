/*
  Button2.cpp - Arduino Library to simplify working with buttons.
  Created by Lennart Hennigs, October 28, 2017.
*/
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

#include "button.h"
#include <unistd.h>
#include <sys/time.h>

#include <stdio.h>

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

Button::Button(uint8_t attachTo, uint8_t buttonMode, uint32_t debounceTimeout)
{
    pin = attachTo;
    setDebounceTime(debounceTimeout);
    bcm2835_gpio_fsel(attachTo, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(attachTo, buttonMode);
}


bool Button::operator==(Button &rhs)
{
    return (this == &rhs);
}


void Button::setDebounceTime(uint32_t ms)
{
    debounce_time_ms = ms;
}


void Button::setChangedHandler(CallbackFunction f)
{
    change_cb = f;
}


void Button::setPressedHandler(CallbackFunction f)
{
    pressed_cb = f;
}


void Button::setReleasedHandler(CallbackFunction f)
{
    released_cb = f;
}


void Button::setClickHandler(CallbackFunction f)
{
    click_cb = f;
}


void Button::setTapHandler(CallbackFunction f)
{
    tap_cb = f;
}


void Button::setLongClickHandler(CallbackFunction f)
{
    long_cb = f;
}


void Button::setDoubleClickHandler(CallbackFunction f)
{
    double_cb = f;
}


void Button::setTripleClickHandler(CallbackFunction f)
{
    triple_cb = f;
}


uint32_t Button::wasPressedFor()
{
    return down_time_ms;
}


bool Button::isPressed()
{
    return (state == LOW);
}


uint32_t Button::getNumberOfClicks()
{
    return click_count;
}


uint32_t Button::getClickType()
{
    return last_click_type;
}


void Button::start(void)
{
    pthread_create(&tid, NULL, threadFunc, (void *)this);
}


long long Button::GetSystemMillis(void)
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return tv.tv_sec * 1000 + tv.tv_usec/1000;
}


void *Button::threadFunc(void *arg)
{
    Button *btn = static_cast<Button *>(arg);

    if (btn->pin == 0xFF) return NULL;

    for (;;)
    {
        btn->prev_state = btn->state;
        btn->state = bcm2835_gpio_lev(btn->pin);

        if (btn->prev_state == HIGH && btn->state == LOW)
        {
            // is button pressed?
            btn->down_ms = btn->GetSystemMillis();
            btn->pressed_triggered = false;
            btn->click_count++;
            btn->click_ms = btn->down_ms;
        }
        else if (btn->prev_state == LOW && btn->state == HIGH)
        {
            // is the button released?
            btn->down_time_ms = btn->GetSystemMillis() - btn->down_ms;
            if (btn->down_time_ms >= btn->debounce_time_ms) // is it beyond debounce time?
            {
                // trigger release
                if (btn->change_cb != NULL)
                    btn->change_cb();
                if (btn->released_cb != NULL)
                    btn->released_cb();
                // trigger tap
                if (btn->tap_cb != NULL)
                    btn->tap_cb();
                // was it a longclick? (preceeds single / double / triple clicks)
                if (btn->down_time_ms >= LONGCLICK_MS)
                {
                    btn->longclick_detected = true;
                }
            }
        }
        else if (btn->state == LOW && !btn->pressed_triggered && \
                 (btn->GetSystemMillis() - btn->down_ms >= btn->debounce_time_ms))
        {
            // trigger pressed event (after debounce has passed)
            if (btn->change_cb != NULL)
                btn->change_cb();
            if (btn->pressed_cb != NULL)
                btn->pressed_cb();
            btn->pressed_triggered = true;
        }
        else if (btn->state == HIGH && \
                 btn->GetSystemMillis() - btn->click_ms > DOUBLECLICK_MS)
        {
            // is the button pressed and the time has passed for multiple clicks?
            // was there a longclick?
            if (btn->longclick_detected)
            {
                // was it part of a combination?
                if (btn->click_count == 1)
                {
                    btn->last_click_type = LONG_CLICK;
                    if (btn->long_cb != NULL)
                        btn->long_cb();
                }
                btn->longclick_detected = false;
            }
            else if (btn->click_count > 0) // determine the number of single clicks
            {
                switch (btn->click_count)
                {
                    case 1:
                        btn->last_click_type = SINGLE_CLICK;
                        if (btn->click_cb != NULL)
                        {
                            btn->click_cb();
                        }
                    break;

                    case 2:
                        btn->last_click_type = DOUBLE_CLICK;
                        if (btn->double_cb != NULL)
                            btn->double_cb();
                    break;

                    case 3:
                        btn->last_click_type = TRIPLE_CLICK;
                        if (btn->triple_cb != NULL)
                            btn->triple_cb();
                    break;
                }
            }
            btn->click_count = 0;
            btn->click_ms = 0;
        }
        usleep(10 * 1000);
    }
}

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/