#include "boards.h"

// Memory
Preferences preferences;

// Global Object/Variable
cSensor mySensor;

// Sensor Variables
RTC_DS3231 rtc;
ACS712 acs712(ACS712_05B, ADC_CURRENT);
ZMPT101B zmpt101b(ADC_VOLTAGE, 50.0);

CRGB leds[NUM_LEDS];
LEDTask led1(0);
LEDTask led2(1);

void initBoard() {
    pinMode(LED_BOARD, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    // DIMER PIN as PWM Pin
    ledcSetup(0, 1000, 8);
    ledcAttachPin(DIMMER_PIN, 0);

    SPI.begin(SPI_SCLK, SPI_MISO, SPI_MOSI);

    FastLED.addLeds<WS2812B, WS2812B_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);

    // Turn relay on
    digitalWrite(RELAY_PIN, LOW);

    leds[0] = CRGB::Blue;
    leds[1] = CRGB::Red;
    FastLED.show();
}
