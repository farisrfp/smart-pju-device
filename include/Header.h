#ifndef HEADER_H
#define HEADER_H

#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include <Wire.h>

#include <FastLED.h>
#include <RTClib.h>
#include <U8g2lib.h>

FASTLED_USING_NAMESPACE

#define WS2812B_PIN 23
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 8
CRGB leds[NUM_LEDS];

void nextPattern();
void rainbow();
void rainbowWithGlitter();
void addGlitter(fract8 chanceOfGlitter);
void confetti();
void sinelon();
void bpm();
void juggle();

#endif