#ifndef BOARDS_H
#define BOARDS_H

#include <ACS712.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <FastLED.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>
#include <ZMPT101B.h>

// Calibration Data
#define ACS712_ZP 2400
#define VOLTAGE_SENSITIVITY 1200.0f

// EEPROM Address
#define EEPROM_SIZE 512
#define ADDR_CURRENT 0
#define ADDR_VOLTAGE 2
#define ADDR_APPEUI 4
#define ADDR_DEVEUI 12
#define ADDR_APPKEY 20

// Debug printing
#define DEBUG_PORT Serial
#if defined(VERBOSE)
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

#include "AP/webserver.h"
#include "LED/led.h"
#include "LoRaWAN/loramac.h"
#include "Sensor/sensor.h"

/* Function */
void initBoard();

/* Pin Mapping */
#define UNUSE_PIN (0)

#ifdef PROTOTYPE

#define I2C_SDA 23
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
#define DIMMER_PIN 13
#define ADC_CURRENT 34
#define ADC_VOLTAGE 35
#define ADC_LIGHT 36
#define LED_BOARD 25

#else

#define I2C_SDA 23
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
#define WS2812B_PIN 14
#define RELAY_PIN 26
#define DIMMER_PIN 27
#define ADC_CURRENT 34
#define ADC_VOLTAGE 35
#define ADC_LIGHT 36
#define LED_BOARD 2

#endif

#endif