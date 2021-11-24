# T-BLOCK

**English | [中文](./README_zh.md)**

## Contents

- [Introduction](#Introduction)
  - [GDEW0371W7](#GDEW0371W7)
    - [GDEW0371W7 Features](#GDEW0371W7%20Features)
  - [FT6336](#FT6336)
- [Quick Start](#Quick%20Start)
- [Image Conversion](#Image%20Conversion)
- [Resources](#Resources)
- [TodoList](#TodoList)

## Introduction

T-BLOCK is a 3.71-inch electronic paper ink screen module, which integrates GooDisplay's GDEW0371W7 and FT6336U touch, onboard three-position toggle buttons, flexibly supports various development platforms, and is fully compatible with rpi.

### GDEW0371W7

This is an electronic paper ink screen, 3.71 inches, 416x240 resolution, 24-pin FPC connection (FPC connector optional), communication via SPI interface, with embedded controller (COG package) and waveforms stored on-chip, Support black and white display, the chip is UC8171.

#### GDEW0371W7 Features

| Product number     | GDEW0371W7       |
| ------------ | ---------------- |
| type         | Graphic dot matrix         |
| IC           | UC8171           |
| Resolution       | 416 * 240(DPI:130) |
| Dimensions(mm) | 92.99 * 53 * 1.18    |
| View field size(mm) | 81.536 * 47.04     |
| Dot pitch(mm)   | 0.196 * 0.196      |
| Operating temperature     | 0 ~ 50℃            |
| Storage temperature     | -25 ~ 70℃          |
| colour         | Black/white             |
| interface         | SPI              |
| Refresh power(mW) | 26.4             |
| Standby power(mW) | 0.09             |
| Full brush time(S) | 4                |
| Maximum gray scale   | 4 grayscale            |

### FT6336

The FT6X36 Series ICs are single-chip capacitive touch panel controller IC with a built-in 16 bit enhanced Micro-controller unit(MCU).They adopt the self-capacitance technology, which supports single point and gesture touch or two points. In conjunction with a self-capacitive touch panel, The FT6X36 Series ICs implement the user-friendly input function and are widely used in various portable devices, such as smart phones, MIDs ad GPS.

## Quick Start

see [examples](./examples)

## Image Conversion

see [Image conversion](./docs/GDEW0371W7gram747b.pdf) and [Image2Lcd](./tools/Image2Lcd).

## Resources

- [GDEW0371W7 Datasheet](./docs/GDEW0371W7%20V1.1%20Specification6329.pdf)
- [UC8171](./docs/UC8171.pdf)
- [Connector Specification](./docs/24Pins-0.5mmconnectorspec5c91.pdf)
- [Image conversion](./docs/GDEW0371W7gram747b.pdf)
- [FT6336](./docs/FT6236-FT6336-FT6436L-FT6436_Datasheet.pdf)

## TodoList

- [ ] Drivrt for python
- [ ] Code comments
- [ ] Image conversion for python
