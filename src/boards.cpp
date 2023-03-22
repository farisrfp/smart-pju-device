#include "PJU.h"
#include "X-Camp.h"

RTC_DS3231 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
CRGB leds[NUM_LEDS];

void initBoard() {
    Serial.begin(115200);

    Wire.begin(I2C_SDA, I2C_SCL);
    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);

    pinMode(BOARD_LED, OUTPUT);
    digitalWrite(BOARD_LED, HIGH);

    rtc.begin();
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    FastLED.addLeds<WS2812B, WS2812B_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);


    for (int dot = 0; dot < NUM_LEDS; dot++) {
        leds[dot] = CRGB::Blue;
        FastLED.show();
        // clear this led for the next time around the loop
        leds[dot] = CRGB::Black;
        delay(100);
    }
    FastLED.show();

#ifdef HAS_DISPLAY
    // Initialize the OLED display
    display.clearDisplay();
    display.drawBitmap(0, 0, X_CAMP_LOGO, 128, 64, 1);
    display.display();
    delay(2500);
    display.setTextSize(1);
    display.setTextColor(WHITE);
#endif
}
