⚠️ This branch is available for those who want to build a remote themself. However, the source code is now obsolete compared to the main branch. ⚠️

# Bluetooth camera remote

The Canon M50 is an excellent Mirrorless camera. However, it misses a descent timelapse mode, one that can do a long exposure. The shutter speed option in M50's timelapse mode is indeed limited to 1/30s.

Furthermore, the M50 doesn't have any shutter release port to plug the classic intervalometers, nor does it supports IR remote. The only options are BLE, WiFi, or tethering tools.
Since the only Bluetooth remote available - BR-E1 - lacks intervalometer mode, let's build our own.

__Functionalities:__
* Time-lapse: shutter triggered at a constant rate, set by the user. The number of photos taken during the current time-lapse and the equivalent video length (24fps) are shown on the screen.
* Single shot: well, that is quite explicit.
* Ext. Trigger: use to take a photo triggered by an external signal connected to the PCB. Can be used for synchronous time-lapse (e.g. 3D print time-lapse).


## Video: In-use

[![Canon M50 | Bluetooth Intervalometer (ESP32)](https://img.youtube.com/vi/HlmzDCDy0Z0/0.jpg)](https://www.youtube.com/watch?v=HlmzDCDy0Z0 "Canon M50 | Bluetooth Intervalometer (ESP32)")

## Electronics

All the components used are available from most electronic resellers :
* [ESP32D - DevKitC](https://docs.espressif.com/projects/esp-idf/en/latest/hw-reference/get-started-devkitc.html "ESP32 guide") : microcontroller BLE compatible
* TP4056: LiPo manager, the charging SMD led is replace by a 3mm led
* Step-up DC-DC converter to 5V
* OLED LCD I2C 1"3 display (128*64): drived by SSG1106 chip (compatible with SSD1306 i.e. compatible with most LCD arduino libraries)
* Lipo battery 1 cell 380mAh (smaller LiPo - 100mAh - would be enough to run for a couple of days) 

![diag_elec](/img/elecDiag.png)

## Software

[Ian Douglas Scott's work on Canon BLE](https://iandouglasscott.com/2018/07/04/canon-dslr-bluetooth-remote-protocol/) saves me from the hassle of reverse engineering this protocol. However, the M50's BLE protocol seems to differ from the T7i. Once the trigger command is sent, the camera stays on triggering mode. I solved this by sending a reset command shortly after the trigger command. One should do further research on the Canon BLE protocol to avoid this kludge.

Libraries used : 
* [BLEDevice](https://github.com/nkolban/ESP32_BLE_Arduino)
* [LCD management](https://github.com/olikraus/u8g2)

## Case

![img_remote](/img/Remote_open.png)
