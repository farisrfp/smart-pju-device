#include "boards.h"

// RTC_DS3231 rtc;

CRGB leds[NUM_LEDS];
LEDTask led1(0);
LEDTask led2(1);

void initBoard() {
    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN, RADIO_CS_PIN);

    FastLED.addLeds<WS2812B, WS2812B_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);

    leds[0] = CRGB::Blue;
    leds[1] = CRGB::Red;
    FastLED.show();
}
