# x86emu

[![Build Status](https://travis-ci.org/shift-crops/x86emu.svg?branch=master)](https://travis-ci.org/shift-crops/x86emu)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE.md)

simple x86 emulator

## Description

x86emu is an emulator of x86 architecture.

This software supports multiple CPU mode (16bit/32bit, RealMode/ProtectedMode), and some devices.
You can boot via FDD simulator (not support DMA), and operate with mouse and keyboard.

![Demo](https://github.com/shift-crops/x86emu/blob/media/demo_170524.gif)

## Features

- Real mode / Protected mode
- Interrupt and Exception
- Devices
- Lightweight BIOS
- Preload FD image function (to reduce boot time)

For more information, see `x86emu --help`.

## Requirement

- [GLFW3](http://www.glfw.org/)

## Installation
    $ git clone https://github.com/shift-crops/x86emu
    $ make all

## Usage

1. Run the emulator with kernel image
    - `./x86emu haribote.img`

## License

[MIT License](LICENSE.md)

## Author

[@shift\_crops](https://twitter.com/shift_crops)

