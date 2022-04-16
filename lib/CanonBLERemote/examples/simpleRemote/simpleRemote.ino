#include "CanonBLERemote.h"
#include <Arduino.h>
#include "Ticker.h"


#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"
#include <esp32-hal-log.h>

#define SHUTTTER_BTN    14
#define FOCUS_BTN   12
#define LED 2

String name_remote = "ESP32 Remote";
CanonBLERemote canon_ble(name_remote);
Ticker blinker;

void blink(){
    digitalWrite(LED, !digitalRead(LED));
}

void setup()
{
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_INFO); 

    pinMode(SHUTTTER_BTN, INPUT_PULLUP);  
    pinMode(FOCUS_BTN, INPUT_PULLUP);   
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    canon_ble.init();
    
    delay(1000);
    // Pairing
    if(digitalRead(SHUTTTER_BTN) == LOW ){
        blinker.attach_ms(200,blink);

        // pair() function should be called only when you want to pair with the new camera. 
        // After paired, the pair() function should not be called.
        do{
            Serial.println("Pairing...");
        }
        while(!canon_ble.pair(10));

        blinker.detach();
        digitalWrite(LED, HIGH);

    }


    delay(1000);
    Serial.println("Setup Done");
}

void loop()
{
  
    // Shutter
    if (digitalRead(SHUTTTER_BTN) == LOW){
        Serial.println("Shutter pressed");
        digitalWrite(LED, LOW);
        if(!canon_ble.trigger()){
            Serial.println("Trigger Failed");
        }
        digitalWrite(LED, HIGH);
    }
    // Focus
    else if (digitalRead(FOCUS_BTN) == LOW){
        Serial.println("Focus pressed");
        digitalWrite(LED, LOW);
        if(!canon_ble.focus()){
            Serial.println("Focus failed");
        }
        digitalWrite(LED, HIGH);

    }

}

