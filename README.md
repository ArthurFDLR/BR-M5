# Bluetooth camera remote

The Canon M50 is a great Mirrorless camera. However it miss one thing, a descent timelapse mode, one that can do long exposure. The shutter speed option in M50's timelapse mode is indeed limited to 1/30s.
Furthermore, the M50 don't have any shutter release port to plug classic intervalometer nor does it supports IR remote. The only options are BLE connection or using a tethering tool.
Since the only Bluetooth remote available (Canon BR-E1) lacks intervalometer mode, here I am, building my own.

![Markdown Logo](/img/Remote_open.png)

## Electronics

All the components used are available from most electronic resellers :
* [ESP32D - DevKitC](https://docs.espressif.com/projects/esp-idf/en/latest/hw-reference/get-started-devkitc.html "ESP32 guide") : microcontroller BLE compatible
* TP4056 : LiPo manager, charging SMD led replace by a 3mm led on case
* Step-up DC-DC converter to 5V
* OLED LCD I2C 1"3 display (128*64) : drived by SSG1106 chip (compatible with SSD1306 i.e. compatible with most LCD arduino libraries)
* Lipo battery 1 cell 380mAh (smaller LiPo like 100mAh would by enough to run for a couple of day) 

## Software

[Ian Douglas Scott's work on Canon BLE](https://iandouglasscott.com/2018/07/04/canon-dslr-bluetooth-remote-protocol/) saves me from the hassle of reverse engineering this protocol. However, the M50's BLE protocol seems to differs from the T7i. Once the trigger command is send, the camera stay on triggering mode. I solved this by sending a reset command shortly after triggering. To avoid this kludge, further research on Canon BLE protocol should be done.

Libraries used : 
* [BLEDevice](https://github.com/nkolban/ESP32_BLE_Arduino)
* [LCD management](https://github.com/olikraus/u8g2)

## Case

I wanted a sturdy remote to carry it during my trips. After modeling a case around the PCB that I allready soldered, I 3D printed it. If you want to build your own, you'll certainly need to adjust the placement of the button on the front pannel.