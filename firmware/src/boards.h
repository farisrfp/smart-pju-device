#ifndef BOARDS_H
#define BOARDS_H

#include <ACS712.h>
#include <Arduino.h>
#include <FastLED.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>
#include <ZMPT101B.h>

// Debug printing
#define DEBUG_PORT Serial
#if defined(_DEBUG_MODE_)
#define DEBUG_PRINTF(...) \
    { DEBUG_PORT.printf(__VA_ARGS__); }
#define DEBUG_PRINTF_TS(...)                                    \
    {                                                           \
        DEBUG_PORT.printf("%d ms: ", osticks2ms(os_getTime())); \
        DEBUG_PORT.printf(__VA_ARGS__);                         \
    }
#else
#define DEBUG_PRINTF(...) \
    {}
#define DEBUG_PRINTF_TS(...) \
    {}
#endif

#include "LED/led.h"
#include "LoRaWAN/loramac.h"
#include "Sensor/sensor.h"
#include "WiFI/wifipoint.h"

/* Global Object/Variable */
extern RTC_DS3231 rtc;
extern CRGB leds[NUM_LEDS];

/* Function */
void initBoard();

/* Pin Mapping for LILYGO_T3_V1_6 */
#define UNUSE_PIN (0)

// #define SPI_SCLK 18
// #define SPI_MISO 5
// #define SPI_MOSI 17
#define SPI_SCLK 5
#define SPI_MISO 19
#define SPI_MOSI 27

// #define PIN_NSS 16
// #define PIN_RST 13
// #define PIN_DIO0 15
// #define PIN_DIO1 2
// #define PIN_DIO2 4
#define PIN_NSS 18
#define PIN_RST 23
#define PIN_DIO0 26
#define PIN_DIO1 33
#define PIN_DIO2 32

// Custom Map
#define WS2812B_PIN 19
#define RELAY_PIN 14
#define ADC1_PIN 34
#define ADC2_PIN 35

#endif