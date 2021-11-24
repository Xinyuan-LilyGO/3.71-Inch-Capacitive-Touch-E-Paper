## 概况

该示例代码基于 GooDisplay 的[示例](#https://www.good-display.cn/companyfile/165.html), 在此基础上做出一些修改以适配 T-BLOCK-3.71inch.

## 快速开始

### 电气连接

| BCM Pin | Raspberry-Pi Pin | Function | Description                |
| ------- | ---------------- | -------- | -------------------------- |
| 4       | 7                | RST      | Reset                      |
| 9       | 21               | DC       | Data/Command control pin   |
| 8       | 24               | CS       | Chip Select input pin(SPI) |
| 25      | 22               | BUSY     | Busy state output pin      |
| 10      | 19               | MOSI     | SPI MOSI pin               |
| 11      | 23               | SCLK     | SPI Clock pin              |

### bcm2835安装

本示例代码依赖 [bcm2835](#http://www.airspayce.com/mikem/bcm2835/) 库, 编译示例代码前, 需要安装该依赖库.

```shell
$ wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.70.tar.gz
$ tar -xvf bcm2835-1.70.tar.gz
$ cd bcm2835-1.70
$ ./configure
$ make && sudo make install
```

### 编译及运行

```shell
$ pwd
/home/pi/3.71-Inch-Capacitive-Touch-E-Paper
$ make all
$ sudo ./bcm2835_epd37
```

> NOTE
>
> 以下操作, 均在rpi 3B实测通过

> NOTE
>
> 因屏幕电源启动需要一定的时间, 所以需要等待10s左右, 才会出现图像.
>
> 使用bcm2835, 需要使用超级用户权限.

## 故障排除

- 如果屏幕无法正常显示图像, 请检查连接口或者屏幕连接排线是否有松动.
