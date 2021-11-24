## Introduction

The sample code based on GooDisplay [example] # (https://www.good-display.cn/companyfile/165.html), on this basis to make some changes to fit T-BLOCK-3.71inch.

## How to Use Example

### Hardware Connection

| BCM Pin | Raspberry-Pi Pin | Function | Description                |
| ------- | ---------------- | -------- | -------------------------- |
| 4       | 7                | RST      | Reset                      |
| 9       | 21               | DC       | Data/Command control pin   |
| 8       | 24               | CS       | Chip Select input pin(SPI) |
| 25      | 22               | BUSY     | Busy state output pin      |
| 10      | 19               | MOSI     | SPI MOSI pin               |
| 11      | 23               | SCLK     | SPI Clock pin              |

### bcm2835 install

This sample code depends on the [bcm2835 library](#http://www.airspayce.com/mikem/bcm2835/). Before compiling the sample code, you need to install the dependent library.

```shell
$ wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.70.tar.gz
$ tar -xvf bcm2835-1.70.tar.gz
$ cd bcm2835-1.70
$ ./configure
$ make && sudo make install
```

### Compile and run

```shell
$ pwd
/home/pi/3.71-Inch-Capacitive-Touch-E-Paper
$ make all
$ sudo ./bcm2835_epd37
```

> NOTE
>
> The above operations are all tested and passed in rpi 3B.

> NOTE
>
> Since it takes a certain amount of time for the EPD to power on, it takes about 10 seconds to display the image.
>
> To use bcm2835, you need to use super user permissions.

## Troubleshooting

- If the screen cannot display the image normally, please check whether the connection port or the screen connection cable is loose.
