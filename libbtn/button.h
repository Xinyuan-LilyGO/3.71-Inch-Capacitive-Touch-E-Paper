
/*
    Button2.cpp - Arduino Library to simplify working with buttons.
    Created by Lennart Hennigs, October 28, 2017.
*/

#ifndef BUTTON_H
#define BUTTON_H

/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include <bcm2835.h>
#include <pthread.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

#define DEBOUNCE_MS       50
#define LONGCLICK_MS      3000
#define DOUBLECLICK_MS    400

#define SINGLE_CLICK      1
#define DOUBLE_CLICK      2
#define TRIPLE_CLICK      3
#define LONG_CLICK        4

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

class Button
{
private:
    uint8_t pin;
    int32_t prev_state;
    int32_t state = HIGH;
    uint8_t click_count = 0;
    uint8_t low_state_count = 0;
    uint32_t last_click_type = 0;
    unsigned long click_ms;
    long long down_ms;
    uint32_t debounce_time_ms;
    uint32_t down_time_ms = 0;
    bool pressed_triggered = false;
    bool longclick_detected = false;

    typedef void (*CallbackFunction) ();

    /**
     * @brief 按键按下回调函数
     **/
    CallbackFunction pressed_cb = NULL;

    /**
     * @brief 按键松开回调函数 
     **/
    CallbackFunction released_cb = NULL;

    /**
     * @brief 按键状态变化回调函数 
     **/
    CallbackFunction change_cb = NULL;

    /**
     * @brief 轻按回调函数 
     **/
    CallbackFunction tap_cb = NULL;

    /**
     * @brief 单击回调函数 
     **/
    CallbackFunction click_cb = NULL;
    
    /**
     * @brief 长按回调函数 
     **/
    CallbackFunction long_cb = NULL;
    
    /**
     * @brief 双击回调函数 
     **/
    CallbackFunction double_cb = NULL;

    /**
     * @brief 三击回调函数 
     **/
    CallbackFunction triple_cb = NULL;

    pthread_t tid;
    long long GetSystemMillis(void);

public:
    Button() {pin = 0xFF;};
    Button(uint8_t attachTo, uint8_t buttonMode = BCM2835_GPIO_PUD_UP, uint32_t debounceTimeout = DEBOUNCE_MS);
    ~Button() {};

    void start(void);
    static void* threadFunc(void *arg);
    void setDebounceTime(uint32_t ms);
    void setChangedHandler(CallbackFunction f);
    void setPressedHandler(CallbackFunction f);
    void setReleasedHandler(CallbackFunction f);
    void setClickHandler(CallbackFunction f);
    void setTapHandler(CallbackFunction f);
    void setLongClickHandler(CallbackFunction f);
    void setDoubleClickHandler(CallbackFunction f);
    void setTripleClickHandler(CallbackFunction f);

    uint32_t wasPressedFor();
    bool isPressed();

    uint32_t getNumberOfClicks();
    uint32_t getClickType();
    uint8_t getAttachPin(){return pin;}
    bool operator==(Button &rhs);

    // void loop();
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