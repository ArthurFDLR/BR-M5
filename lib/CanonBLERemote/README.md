[![Bluetooth Remote for Modern Canon Cameras](./.github/cover.jpg)](https://www.youtube.com/watch?v=mM_tIqrD_5A "ESP32 Canon BLE Remote Library Demo")
[Demo Video](https://www.youtube.com/watch?v=mM_tIqrD_5A)

# ESP32 Canon BLE Remote Library
This Bluetooth remote library aims to replicate Canon's BR-E1 bluetooth remote behavior, since modern Canon Cameras doesn't have wired shutter trigger port anymore. Ex. EOS M50, EOS R, EOS EP.

This library is adapted and inspired from [BR-M5](https://github.com/ArthurFDLR/BR-M5)

## Features
* Single firing and focus commands.
* Pair and remember. (Paring is only required for initial connection)
* Auto re-connect.

## Installation
1. Install [ArduinoNvs]() library, uses to store paring data to ESP32 NVS storage.
2. Install this library from the following way.
  - [Platform IO Library Manager](https://platformio.org/lib/show/12410/Canon%20BLE%20Remote/)
  - Arduino IDE Library Manager
  - Manual installation by copying this repository to your `library` folder

## Usage
I'm highly recommend you to checkout [example code](https://github.com/maxmacstn/ESP32-Canon-BLE-Remote/blob/master/examples/simpleRemote/simpleRemote.ino) to see how to use it.

1.  On camera, go to Wireless Communication Settings > Bluetooth Function > set bluetooth function to Remote. Clear all existing connection (if necesary) and press Pairing.
2.  Call `pair()` function. If you're trying example code, press shutter button while booting ESP32 to enter pairing mode.
3.  For picture mode, enable remote shutter in drive mode menu (Self-timer:10s/Remote). For video mode, press menu button and enable remote control.

If paring doesn’t work, clear all existing connection, power off and re-insert battery (necessary), then try again. 

## Schematic for testing
[![example code](./.github/demo_diagram.jpg)](https://github.com/maxmacstn/ESP32-Canon-BLE-Remote/blob/master/examples/simpleRemote/simpleRemote.ino)


## Tested device(s)
- Canon EOS M50 (EOS Kiss M)

(Feel free to help me test with other cameras, but technically, it should work)

## Background Info
- [CB Remote Android App](https://github.com/iebyt/cbremote)
- [Canon DSLR Bluetooth Remote Protocol Reverse Engineering](https://iandouglasscott.com/2018/07/04/canon-dslr-bluetooth-remote-protocol/)

## To-do
- [ ] Add support for W/T buttons.
- [ ] Optimize and cleanup code.
- [ ] Eliminate other dependency.

Feel free to contribute!
