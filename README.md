# Bluetooth camera remote

The Canon M50 is a great Mirrorless camera. However it miss one thing, a descent timelapse mode, one that can do long exposure. The shutter speed option in M50's timelapse mode is indeed limited to 1/30s.

Furthermore, the M50 don't have any shutter release port to plug classic intervalometer nor does it supports IR remote. The only options are BLE connection or using a tethering tool.
Since the only Bluetooth remote available - BR-E1(A) - lacks intervalometer mode, here I am, building my own.

__Functionalities:__
* Time-lapse: shutter trigerred at a constant rate, set by the user. The numbre of photos taken during the current time-lapse is shown on the screen as well as the video length equivalent at 24fps.
* Single shot: well, that is quite explicit.
* Ext. trigger: use to take a photo triggered by an external signal connected to the PCB. Can be use for synchronous time-lapse (eg. 3D print time-lapse).


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

[Ian Douglas Scott's work on Canon BLE](https://iandouglasscott.com/2018/07/04/canon-dslr-bluetooth-remote-protocol/) saves me from the hassle of reverse engineering this protocol. However, the M50's BLE protocol seems to differs from the T7i. Once the trigger command is send, the camera stay on triggering mode. I solved this by sending a reset command shortly after triggering. To avoid this kludge, further research on Canon BLE protocol should be done.

Libraries used : 
* [BLEDevice](https://github.com/nkolban/ESP32_BLE_Arduino)
* [LCD management](https://github.com/olikraus/u8g2)

## Case

I wanted a sturdy remote to carry it during my trips. After modeling a case around the PCB that I allready soldered, I 3D printed it. If you want to build your own, you'll certainly need to adjust the placement of the button on the front pannel.

![img_remote](/img/Remote_open.png)
