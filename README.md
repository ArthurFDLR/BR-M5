# BR-M5: An open-source Bluetooth Remote for Canon DSLRs

This Bluetooth remote aims to replicate Canon's BR-E1 behavior while adding key features like fully programable time-lapses that most Canon cameras lack! Fortunately, *M5Stack* proposes a nice little inexpensive development board that perfectly fits our needs, the [M5Stick-Cplus](https://m5stack.com/products/m5stickc-plus-esp32-pico-mini-iot-development-kit?variant=35275856609444).

The BR-M5 has been primarily developed for the Canon M50 Mark I, which misses a descent timelapse mode that can do long exposures. The shutter speed option in M50's timelapse mode is indeed limited to 1/30s. Furthermore, the M50 doesn't have any shutter release port to plug classic intervalometers. No official Bluetooth, WiFi, or tethering tools currently offer time-lapse capabilities.

## Installation

* Install [Visual Studio Code](https://code.visualstudio.com/download) and the [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) extension.
* Clone this repository - ```git clone https://github.com/ArthurFDLR/br-m5``` -, open it as project in PlatformIO and upload - ```Ctrl+Alt+u``` - to a M5Stick-Cplus connected to your PC.

## How-To-Use

* Power on by pressing the left-side button and power off with a 6 seconds press on the same button.
* You can connect to your Canon DSLR as soon as the remote is powered on and displays *Pairing in progress*.
* To change the interval, press the right-side button for half a second and use the same button to increase and the center button to decrease the timelapse interval. Once set, press the right-side button for half a second again.
* Start and stop time-lapses using the center button. The remote takes single shots if the interval is set to 0 seconds.

## To-do

- [ ] Fix connection reliability issues.
- [ ] Auto connection to camera when remote start-up.
- [ ] Add battery level indication.

Feel free to contribute!
