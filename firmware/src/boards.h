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
#if defined(DEBUG_MODE)
#define DEBUG_PRINTF(...) \
    { DEBUG_PORT.printf(__VA_ARGS__); }
#define DEBUG_PRINTF_TS(...)                                    \
    {                                                           \
        DEBUG_PORT.printf("%d ms: ", osticks2ms(os_getTime())); \
        DEBUG_PORT.printf(__VA_ARGS__);                        \
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

/* Function */
void initBoard();

/* Pin Mapping */
#define UNUSE_PIN (0)

#ifdef PROTOTYPE

#define I2C_SDA 21
#define I2C_SCL 22

#define SPI_SCLK 5
#define SPI_MISO 19
#define SPI_MOSI 27
#define PIN_NSS 18
#define PIN_RST 23
#define PIN_DIO0 26
#define PIN_DIO1 33
#define PIN_DIO2 32

// Other
#define WS2812B_PIN 14
#define RELAY_PIN 15
#define ADC1_PIN 34
#define ADC2_PIN 35

#else

#define I2C_SDA 21
#define I2C_SCL 22

#define SPI_SCLK 18
#define SPI_MISO 5
#define SPI_MOSI 17
#define PIN_NSS 16
#define PIN_RST 13
#define PIN_DIO0 15
#define PIN_DIO1 2
#define PIN_DIO2 4
// Other
#define WS2812B_PIN 19
#define RELAY_PIN 14
#define ADC1_PIN 34
#define ADC2_PIN 35

#endif

#endif