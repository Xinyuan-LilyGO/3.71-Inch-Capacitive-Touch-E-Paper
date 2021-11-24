#ifndef __FT6336_H
#define __FT6336_H

/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include <pthread.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

#define TP_INT_PIN   7

#define TP_PRES_DOWN 0x80 //触屏被按下
#define TP_COORD_UD  0x40 //触摸坐标更新标记

#define I2C_ADDR_FT6336 0x38

/**
 * @brief FT6336 部分寄存器定义
 */
typedef enum te_WorkingModeRegisterMap
{
    E_FT6336_REG_DEV_MODE              = 0x00, /**< This is the device mode register, which is configured to determine the current mode of the chip. */
    E_FT6336_REG_GEST_ID               = 0x01, /**< This register describes the gesture of a valid touch. */
    E_FT6336_REG_TD_STATUS             = 0x02, /**< This register is the Touch Data status register. */
    E_FT6336_REG_P1_XH                 = 0x03, /**< This register describes MSB of the X coordinate of the nth touch point and the corresponding event flag. */
    E_FT6336_REG_P1_XL                 = 0x04, /**< This register describes LSB of the X coordinate of the nth touch point. */
    E_FT6336_REG_P1_YH                 = 0x05, /**< This register describes MSB of the Y coordinate of the nth touch point and corresponding touch ID. */
    E_FT6336_REG_P1_YL                 = 0x06, /**< This register describes LSB of the Y coordinate of the nth touch point. */
    E_FT6336_REG_P1_WEIGHT             = 0x07, /**< This register describes weight of the nth touch point. */
    E_FT6336_REG_P1_MISC               = 0x08, /**< This register describes the miscellaneous information of the nth touch point. */
    E_FT6336_REG_P2_XH                 = 0x09,
    E_FT6336_REG_P2_XL                 = 0x0A,
    E_FT6336_REG_P2_YH                 = 0x0B,
    E_FT6336_REG_P2_YL                 = 0x0C,
    E_FT6336_REG_P2_WEIGHT             = 0x0D,
    E_FT6336_REG_P2_MISC               = 0x0E,
    E_FT6336_REG_TH_GROUP              = 0x80, /**< 触摸有效值设置寄存器 */
    E_FT6336_REG_TH_DIFF               = 0x85,
    E_FT6336_REG_CTRL                  = 0x86,
    E_FT6336_REG_TIMEENTERMONITOR      = 0x87,
    E_FT6336_REG_PERIODACTIVE          = 0x88,
    E_FT6336_REG_PERIODMONITOR         = 0x89,
    E_FT6336_REG_RADIAN_VALUE          = 0x91,
    E_FT6336_REG_OFFSET_LEFT_RIGHT     = 0x92,
    E_FT6336_REG_OFFSET_UP_DOWN        = 0x93,
    E_FT6336_REG_DISTANCE_LEFT_RIGHT   = 0x94,
    E_FT6336_REG_DISTANCE_UP_DOWN      = 0x95,
    E_FT6336_REG_DISTANCE_ZOOM         = 0x96,
    E_FT6336_REG_LIB_VER_H             = 0xA1,
    E_FT6336_REG_LIB_VER_L             = 0xA2,
    E_FT6336_REG_CIPHER                = 0xA3,
    E_FT6336_REG_G_MODE                = 0xA4,
    E_FT6336_REG_PWR_MODE              = 0xA5,
    E_FT6336_REG_FIRMID                = 0xA6,
    E_FT6336_REG_FOCALTECH_ID          = 0xA8,
    E_FT6336_REG_RELEASE_CODE_ID       = 0xAF,
    E_FT6336_REG_STATE                 = 0xBC,
} te_WorkingModeRegisterMap;

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

/**
 * @brief 触摸点相关数据结构体定义
 */
typedef struct ts_TouchPointData
{
    uint8_t  u8Status;
    uint8_t u8TouchId;
    uint16_t x;
    uint16_t y;
} ts_TouchPointData;


class FT6336
{
private:
    int8_t i8DeviceAddr = 0;
    uint32_t u32Baudrat = 0;
    pthread_t tid;
    uint8_t pin = 0xFF;
    ts_TouchPointData asTouchPointData[2];
    typedef void (*CallbackFunction) (uint16_t x, uint16_t y);
    /**
     * @brief 按键按下回调函数
     **/
    CallbackFunction pressed_cb = NULL;

public:
    FT6336(int8_t i8DeviceAddr = I2C_ADDR_FT6336, uint32_t u32Baudrate = 400000, uint8_t u8INTPin = TP_INT_PIN);
    ~FT6336();

    uint8_t ReadAddrByte(uint8_t u8RegAddr);
    void ReadAddrBuffer(uint8_t u8RegAddr, uint8_t u8Length, uint8_t *u8ReadBuf);
    void WriteRegByte(uint8_t u8RegAddr, uint8_t u8Value);

    void WriteDeviceMode(uint8_t u8DeviceMode);
    uint8_t ReadDeviceMode(void);
    uint8_t ReadGestureId(void);
    uint8_t ReadTDStatus(void);
    //第一触点
    uint8_t ReadTouch1_Event(void);
    uint8_t ReadTouch1Id(void);
    uint16_t ReadTouch1X(void);
    uint16_t ReadTouch1Y(void);
    //第二触点
    uint8_t ReadTouch2Event(void);
    uint8_t ReadTouch2Id(void);
    uint16_t ReadTouch2X(void);
    uint16_t ReadTouch2Y(void);
    void WriteThresholdForTouchDetection(uint8_t u8Threshold);
    uint8_t ReadThresholdForTouchDetection(void);
    void WriteFilterFunctionCoefficient(uint8_t u8Coefficient);
    uint8_t ReadFilterFunctionCoefficient(void);
    void WriteCtrlMode(uint8_t u8CtrlMode);
    uint8_t ReadCtrlMode(void);
    void WriteTimeEnterMonitor(uint8_t u8TimePeriod);
    uint8_t ReadTimeEnterMonitor(void);
    void WritePeriodActive(uint8_t u8ReportRate);
    uint8_t ReadPeriodActive(void);
    void WritePeriodMonitor(uint8_t u8ReportRate);
    uint8_t ReadPeriodMonitor(void);
    void WriteRadianValue(uint8_t u8Value);
    uint8_t ReadRadianValue(void);
    void WriteOffsetLeftRight(uint8_t u8Offset);
    uint8_t ReadOffsetLeftRight(void);
    void WriteOffsetUpDown(uint8_t u8Offset);
    uint8_t ReadOffsetUpDown(void);
    void WriteDistanceLeftRight(uint8_t u8Distance);
    uint8_t ReadDistanceLeftRight(void);
    void WriteDistanceUpDown(uint8_t u8Distance);
    uint8_t ReadDistanceUpDown(void);
    void WriteDistanceZoom(uint8_t u8Distance);
    uint8_t ReadDistanceZoom(void);
    uint16_t ReadLIBVersion(void);
    uint8_t ReadChipSelecting(void);
    void WriteGMode(uint8_t u8Mode);
    uint8_t ReadGMode(void);
    void WritePowerMode(uint8_t u8Mode);
    uint8_t ReadPowerMode(void);
    uint8_t ReadFirmwareId(void);
    uint8_t ReadFocaltechId(void);
    uint8_t ReadReleaseCodeId(void);
    uint8_t ReadState(void);

    //静态方法
    static void* threadScan(void *arg);

    // 应用层方法
    void setPressedHandler(CallbackFunction f);
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