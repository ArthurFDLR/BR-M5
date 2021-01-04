//! Copyright (c) M5Stack. All rights reserved.

//! Licensed under the MIT license. See LICENSE file in the project root for full license information.
/**
 * \par Copyright (C), 2016-2017, M5Stack
 * \class M5StickC
 * \brief   M5StickCPlus library.
 * @file    M5StickCPlus.h
 * @author  M5Stack
 * @version V0.0.6
 * @date    2020/07/16
 * @brief   Header for M5StickCPlus.cpp module
 *
 * \par Description
 * This file is a drive for M5StickCPlus.
 *
 * \par Method List:
 *    
 *  System:
 *    M5.begin();
 * 
 *  LCD:
        M5.Lcd.drawPixel(int16_t x, int16_t y, uint16_t color);
        M5.Lcd.drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        M5.Lcd.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        M5.Lcd.fillScreen(uint16_t color);
        M5.Lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        M5.Lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,uint16_t color);
        M5.Lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        M5.Lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta, uint16_t color);
        M5.Lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        M5.Lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        M5.Lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        M5.Lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        M5.Lcd.drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
        M5.Lcd.drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h),
        M5.Lcd.drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
        M5.Lcd.setCursor(uint16_t x0, uint16_t y0);
        M5.Lcd.setTextColor(uint16_t color);
        M5.Lcd.setTextColor(uint16_t color, uint16_t backgroundcolor);
        M5.Lcd.setTextSize(uint8_t size);
        M5.Lcd.setTextWrap(boolean w);
        M5.Lcd.printf();
        M5.Lcd.print();
        M5.Lcd.println();
        M5.Lcd.drawCentreString(const char *string, int dX, int poY, int font);
        M5.Lcd.drawRightString(const char *string, int dX, int poY, int font);
        //M5.Lcd.drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x, uint16_t y);
        //M5.Lcd.drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
        //M5.Lcd.drawBmpFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
 * 
 * *
 * \par History:
 * <pre>
 * `<Author>`       `<Time>`        `<Version>`        `<Descr>`
 * Hades           2020/07/16         0.0.1          creat new library.
 * </pre>
 *
 */

#ifndef _M5STICKC_H_
#define _M5STICKC_H_

#if defined(ESP32)

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "M5Display.h"
#include "AXP192.h"

#include "utility/Config.h"
#include "utility/MPU6886.h"
#include "utility/Button.h"
#include "utility/Speaker.h"
#include "RTC.h"


class M5StickCPlus {

 public:
    M5StickCPlus();
	void begin(bool LCDEnable=true, bool PowerEnable=true, bool SerialEnable=true);
    void update();

    //!LCD
    M5Display Lcd = M5Display();

    //!Power
    AXP192 Axp = AXP192();
    SPEAKER Beep;

    #define DEBOUNCE_MS 10
    Button BtnA = Button(BUTTON_A_PIN, true, DEBOUNCE_MS);
    Button BtnB = Button(BUTTON_B_PIN, true, DEBOUNCE_MS);
    //!RTC
    RTC  Rtc;
    MPU6886 Imu;

 private:
    bool isInited;
    //uint8_t _wakeupPin;
};


extern M5StickCPlus M5;
#define m5 M5
#define lcd Lcd
#define axp Axp
#define imu Imu
#define IMU Imu

#else
#error “This library only supports boards with ESP32 processor.”	
#endif

#endif