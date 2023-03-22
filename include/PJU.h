#ifndef PJU_H
#define PJU_H

#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>

/* Struct */
struct sensor {
    float temperature;
    float voltage;
    int current;
    int light;
    DateTime time;
};

#define NUM_LEDS 2
#define BRIGHTNESS 96
#define FRAMES_PER_SECOND 120

/* Global Object/Variable */
extern Adafruit_SSD1306 display;
extern RTC_DS3231 rtc;
extern sensor sensor_data;
extern CRGB leds[NUM_LEDS];

extern uint8_t gHue;                   // rotating "base color" used by many of the patterns
extern uint8_t gCurrentPatternNumber;  // Index number of which pattern is currentD
typedef void (*SimplePatternList[])();
extern SimplePatternList gPatterns;

/* Function */
void initBoard();

/* Pin Mapping for LILYGO_T3_V1_6 */
#define UNUSE_PIN (0)

#define I2C_SDA 21
#define I2C_SCL 22
#define OLED_RST UNUSE_PIN

#define RADIO_SCLK_PIN 5
#define RADIO_MISO_PIN 19
#define RADIO_MOSI_PIN 27
#define RADIO_CS_PIN 18
#define RADIO_DIO0_PIN 26
#define RADIO_RST_PIN 23
#define RADIO_DIO1_PIN 33
#define RADIO_BUSY_PIN 32

#define SDCARD_MOSI 15
#define SDCARD_MISO 2
#define SDCARD_SCLK 14
#define SDCARD_CS 13

#define BOARD_LED 25

#define WS2812B_PIN 2

#define ADC_PIN 35

#define HAS_DISPLAY
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

#endif