#include "PJU.h"
#include "X-Camp.h"

RTC_DS3231 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
CRGB leds[NUM_LEDS];
LEDTask led1(0);
LEDTask led2(1);

void initBoard() {
    Serial.begin(115200);

    Wire.begin(I2C_SDA, I2C_SCL);
    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);

    pinMode(BOARD_LED, OUTPUT);
    pinMode(ADC_PIN, INPUT);
    digitalWrite(BOARD_LED, HIGH);

    rtc.begin();
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    FastLED.addLeds<WS2812B, WS2812B_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);

    leds[0] = CRGB::Blue;
    leds[1] = CRGB::Black;
    FastLED.show();

#ifdef HAS_DISPLAY
    // Initialize the OLED display
    display.clearDisplay();
    display.drawBitmap(0, 0, X_CAMP_LOGO, 128, 64, 1);
    display.display();
    delay(3000);
    display.setTextSize(1);
    display.setTextColor(WHITE);
#endif
}
