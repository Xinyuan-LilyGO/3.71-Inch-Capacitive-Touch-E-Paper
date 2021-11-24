/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "FT6336.h"
#include <bcm2835.h>
#include <unistd.h>
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

FT6336::FT6336(int8_t i8DeviceAddr, uint32_t u32Baudrate, uint8_t u8INTPin)
{
    this->i8DeviceAddr = i8DeviceAddr;
    this->u32Baudrat = u32Baudrate;

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(i8DeviceAddr);
    bcm2835_i2c_set_baudrate(u32Baudrate);

    //进入正常操作模式
    WriteDeviceMode(0);

    //设置触摸有效值
    WriteThresholdForTouchDetection(5);

    //激活周期，不能小于12，最大14
    WriteTimeEnterMonitor(12);

    // 初始化INT io
    bcm2835_gpio_fsel(u8INTPin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(u8INTPin, BCM2835_GPIO_PUD_UP);
    this->pin = u8INTPin;
    pthread_create(&tid, NULL, threadScan, (void *)this);
}


FT6336::~FT6336(void)
{
    bcm2835_i2c_end();
}


uint8_t FT6336::ReadAddrByte(uint8_t u8RegAddr)
{
    uint8_t dat = 0;

    bcm2835_i2c_read_register_rs((char *)&u8RegAddr, (char *)&dat, 1);
    return (dat);
}


void FT6336::ReadAddrBuffer(uint8_t u8RegAddr, uint8_t u8Length, uint8_t *u8ReadBuf)
{
    bcm2835_i2c_read_register_rs((char *)&u8RegAddr, (char *)u8ReadBuf, u8Length);
}


void FT6336::WriteRegByte(uint8_t u8RegAddr, uint8_t u8Value)
{
    int8_t au8Payload[3] = { 0 };

    //au8Payload[0] = FT_CMD_WR;
    au8Payload[0] = u8RegAddr;
    au8Payload[1] = u8Value;

    bcm2835_i2c_write((const char *)au8Payload, 2);
}


void FT6336::WriteDeviceMode(uint8_t u8DeviceMode)
{
    WriteRegByte(E_FT6336_REG_DEV_MODE, u8DeviceMode);
}


uint8_t FT6336::ReadDeviceMode(void)
{
    return (ReadAddrByte(E_FT6336_REG_DEV_MODE) & 0xFF) >> 4;
}


uint8_t FT6336::ReadGestureId(void)
{
    return ReadAddrByte(E_FT6336_REG_GEST_ID);
}


uint8_t FT6336::ReadTDStatus(void)
{
    return ReadAddrByte(E_FT6336_REG_TD_STATUS) & 0x0F;
}


//第一触点
uint8_t FT6336::ReadTouch1_Event(void)
{
    return ReadAddrByte(E_FT6336_REG_P1_XH) >> 4;
}


uint8_t FT6336::ReadTouch1Id(void)
{
    return ReadAddrByte(E_FT6336_REG_P1_YH) >> 4;
}


uint16_t FT6336::ReadTouch1X(void)
{
    uint8_t read_buf[2];
    ReadAddrBuffer(E_FT6336_REG_P1_XH, 2, read_buf);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}


uint16_t FT6336::ReadTouch1Y(void)
{
    uint8_t read_buf[2];
    ReadAddrBuffer(E_FT6336_REG_P1_YH, 2, read_buf);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}


//第二触点
uint8_t FT6336::ReadTouch2Event(void)
{
    return ReadAddrByte(E_FT6336_REG_P2_XH) >> 4;
}


uint8_t FT6336::ReadTouch2Id(void)
{
    return ReadAddrByte(E_FT6336_REG_P2_YH) >> 4;
}


uint16_t FT6336::ReadTouch2X(void)
{
    uint8_t read_buf[2];
    ReadAddrBuffer(E_FT6336_REG_P2_XH, 2, read_buf);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}


uint16_t FT6336::ReadTouch2Y(void)
{
    uint8_t read_buf[2];
    ReadAddrBuffer(E_FT6336_REG_P2_YH, 2, read_buf);
    return ((read_buf[0] & 0x0f) << 8) | read_buf[1];
}


//触摸有效值，22，越小越灵敏
void FT6336::WriteThresholdForTouchDetection(uint8_t u8Threshold)
{
    WriteRegByte(E_FT6336_REG_TH_GROUP, u8Threshold);
}


uint8_t FT6336::ReadThresholdForTouchDetection(void)
{
    return ReadAddrByte(E_FT6336_REG_TH_GROUP);
}


void FT6336::WriteFilterFunctionCoefficient(uint8_t u8Coefficient)
{
    WriteRegByte(E_FT6336_REG_TH_DIFF, u8Coefficient);
}


uint8_t FT6336::ReadFilterFunctionCoefficient(void)
{
    return ReadAddrByte(E_FT6336_REG_TH_DIFF);
}


void FT6336::WriteCtrlMode(uint8_t u8CtrlMode)
{
    WriteRegByte(E_FT6336_REG_CTRL, u8CtrlMode);
}


uint8_t FT6336::ReadCtrlMode(void)
{
    return ReadAddrByte(E_FT6336_REG_CTRL);
}


void FT6336::WriteTimeEnterMonitor(uint8_t u8TimePeriod)
{
    WriteRegByte(E_FT6336_REG_TIMEENTERMONITOR, u8TimePeriod);
}


uint8_t FT6336::ReadTimeEnterMonitor(void)
{
    return ReadAddrByte(E_FT6336_REG_TIMEENTERMONITOR);
}


void FT6336::WritePeriodActive(uint8_t u8ReportRate)
{
    WriteRegByte(E_FT6336_REG_PERIODACTIVE, u8ReportRate);
}


uint8_t FT6336::ReadPeriodActive(void)
{
    return ReadAddrByte(E_FT6336_REG_PERIODACTIVE);
}


void FT6336::WritePeriodMonitor(uint8_t u8ReportRate)
{
    WriteRegByte(E_FT6336_REG_PERIODMONITOR, u8ReportRate);
}


uint8_t FT6336::ReadPeriodMonitor(void)
{
    return ReadAddrByte(E_FT6336_REG_PERIODMONITOR);
}


void FT6336::WriteRadianValue(uint8_t u8Value)
{
    WriteRegByte(E_FT6336_REG_RADIAN_VALUE, u8Value);
}


uint8_t FT6336::ReadRadianValue(void)
{
    return ReadAddrByte(E_FT6336_REG_RADIAN_VALUE);
}


void FT6336::WriteOffsetLeftRight(uint8_t u8Offset)
{
    WriteRegByte(E_FT6336_REG_OFFSET_LEFT_RIGHT, u8Offset);
}


uint8_t FT6336::ReadOffsetLeftRight(void)
{
    return ReadAddrByte(E_FT6336_REG_OFFSET_LEFT_RIGHT);
}


void FT6336::WriteOffsetUpDown(uint8_t u8Offset)
{
    WriteRegByte(E_FT6336_REG_OFFSET_UP_DOWN, u8Offset);
}


uint8_t FT6336::ReadOffsetUpDown(void)
{
    return ReadAddrByte(E_FT6336_REG_OFFSET_UP_DOWN);
}


void FT6336::WriteDistanceLeftRight(uint8_t u8Distance)
{
    WriteRegByte(E_FT6336_REG_DISTANCE_LEFT_RIGHT, u8Distance);
}


uint8_t FT6336::ReadDistanceLeftRight(void)
{
    return ReadAddrByte(E_FT6336_REG_DISTANCE_LEFT_RIGHT);
}


void FT6336::WriteDistanceUpDown(uint8_t u8Distance)
{
    WriteRegByte(E_FT6336_REG_DISTANCE_UP_DOWN, u8Distance);
}


uint8_t FT6336::ReadDistanceUpDown(void)
{
    return ReadAddrByte(E_FT6336_REG_DISTANCE_UP_DOWN);
}


void FT6336::WriteDistanceZoom(uint8_t u8Distance)
{
    WriteRegByte(E_FT6336_REG_DISTANCE_ZOOM, u8Distance);
}


uint8_t FT6336::ReadDistanceZoom(void)
{
    return ReadAddrByte(E_FT6336_REG_DISTANCE_ZOOM);
}


uint16_t FT6336::ReadLIBVersion(void)
{
    uint16_t u16Version = 0;

    u16Version = ReadAddrByte(E_FT6336_REG_LIB_VER_H) << 8;
    u16Version |= ReadAddrByte(E_FT6336_REG_LIB_VER_L);

    return u16Version;
}


uint8_t FT6336::ReadChipSelecting(void)
{
    return ReadAddrByte(E_FT6336_REG_CIPHER);
}


void FT6336::WriteGMode(uint8_t u8Mode)
{
    WriteRegByte(E_FT6336_REG_G_MODE, u8Mode);
}


uint8_t FT6336::ReadGMode(void)
{
    return ReadAddrByte(E_FT6336_REG_G_MODE);
}


void FT6336::WritePowerMode(uint8_t u8Mode)
{
    WriteRegByte(E_FT6336_REG_PWR_MODE, u8Mode);
}


uint8_t FT6336::ReadPowerMode(void)
{
    return ReadAddrByte(E_FT6336_REG_PWR_MODE);
}


uint8_t FT6336::ReadFirmwareId(void)
{
    return ReadAddrByte(E_FT6336_REG_FIRMID);
}


uint8_t FT6336::ReadFocaltechId(void)
{
    return ReadAddrByte(E_FT6336_REG_FOCALTECH_ID);
}


uint8_t FT6336::ReadReleaseCodeId(void)
{
    return ReadAddrByte(E_FT6336_REG_RELEASE_CODE_ID);
}


uint8_t FT6336::ReadState(void)
{
    return ReadAddrByte(E_FT6336_REG_STATE);
}


const uint16_t FT6236_TPX_TBL[2] =
{
    E_FT6336_REG_P1_XH,
    E_FT6336_REG_P2_XH
};


//左下角为坐标零点,水平Y轴,垂直X轴
//Y轴范围0~459
//X轴范围0~319
//touch1和touch2排列顺序为按面积排列,而ID号为按下的时间先后标号,该标号可代表按下的手指
//在第一点按下后ID记为0,事件为8,而抬起后ID与坐标保持最后数值,事件为4,此时第二个点数据一直为FF
//在第一点按下前提下按第二个点,ID记为1,事件为8,抬起后全恢复FF,而当第二个点的接触面积大于第一个点时,
//它将保持ID与坐标前提下与第一个点更换存储地址,第一个点抬起后,第二个点的依旧ID为1
//按下第三个点时,将与其它两个点进行面积对比后,直接抛弃面积最小的点
//
// Touch 坐标轴
//
// --------------------------
// | (0, 0)        (319, 0) |
// |                        |
// |                        |
// |                        |
// |                        |
// |                        |
// |                        |
// |                        |
// |                        |
// |                        |
// |                        |
// |                        |
// |                        |
// |                        |
// | (0, 459)    (319, 459) |
// --------------------------
// | FPC排线 |
//
void *FT6336::threadScan(void *arg)
{
    FT6336 *tp = static_cast<FT6336 *>(arg);

    uint8_t i = 0;
    uint8_t sta = 0;
    uint8_t buf[4] = {0};
    uint8_t gestid = 0;

    if (tp->pin == 0xFF) return NULL;

    printf("thread start\n");

    for (;;)
    {
        if(bcm2835_gpio_lev(tp->pin) != 0)
        {
            usleep(1 * 1000);
            continue;
        }
        sta = tp->ReadTDStatus();
        gestid = tp->ReadGestureId();

        if (sta) //判断是否有触摸点按下，0x02寄存器的低4位表示有效触点个数
        {
            if (tp->ReadTouch1_Event() == 0x08)
            {
                tp->asTouchPointData[0].u8TouchId = tp->ReadTouch1Id();
                tp->asTouchPointData[0].x = tp->ReadTouch1X();
                tp->asTouchPointData[0].y = tp->ReadTouch1Y();
                // printf("x1: %d, y1: %d\n", tp->asTouchPointData[0].x, tp->asTouchPointData[0].y);
                if (tp->pressed_cb)
                    tp->pressed_cb(tp->asTouchPointData[0].x, tp->asTouchPointData[0].y);
            }
            if (tp->ReadTouch2Event() == 0x08)
            {
                tp->asTouchPointData[1].u8TouchId = tp->ReadTouch2Id();
                tp->asTouchPointData[1].x = tp->ReadTouch2X();
                tp->asTouchPointData[1].y = tp->ReadTouch2Y();
                // printf("x2: %d, y2: %d\n", tp->asTouchPointData[1].x, tp->asTouchPointData[1].y);
            }
        }
        else
        {
            tp->asTouchPointData[0].u8TouchId = 0xFF;
            tp->asTouchPointData[0].x = 0xFFFF;
            tp->asTouchPointData[0].y = 0xFFFF;
            tp->asTouchPointData[1].u8TouchId = 0xFF;
            tp->asTouchPointData[1].x = 0xFFFF;
            tp->asTouchPointData[1].y = 0xFFFF;
        }
    }
}


void FT6336::setPressedHandler(CallbackFunction f)
{
    this->pressed_cb = f;
}

/******************************************************************************/
/***        local functions                                                 ***/
/******************************************************************************/

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/