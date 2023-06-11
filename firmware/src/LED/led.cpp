#include "led.h"

LEDTask::LEDTask(uint8_t led) {
    this->led = led;
    this->mode = OFF;

    // Start task.
    xTaskCreate(
        [](void *obj) -> void {
            static_cast<LEDTask *>(obj)->Task(obj);
        },
        String("LEDTask" + led).c_str(),
        1000,
        this,
        0,
        NULL);
}

void LEDTask::Task(void *pvParameters) {
    int fadeTime;

    for (;;) {
        switch (mode) {
            case FADE:
                brightness += brightnessStep;

                if (color == RED) leds[led] = CRGB(brightness, 0, 0);
                if (color == GREEN) leds[led] = CRGB(0, brightness, 0);
                if (color == BLUE) leds[led] = CRGB(0, 0, brightness);

                if (brightness >= 255 || brightness <= 0) {
                    fadeTime = brightness >= 255 ? offTime : onTime;
                    brightnessStep = -brightnessStep;
                }

                FastLED.show();
                vTaskDelay(fadeTime / portTICK_PERIOD_MS);
                break;
            case FLICKER:
                leds[led] = color;
                FastLED.show();
                vTaskDelay(onTime / portTICK_PERIOD_MS);

                leds[led] = CRGB::Black;
                FastLED.show();
                vTaskDelay(offTime / portTICK_PERIOD_MS);
                break;
            case ON:
                leds[led] = color;
                FastLED.show();
                vTaskDelay(500 / portTICK_PERIOD_MS);
                break;
            case OFF_AFTER:
                leds[led] = color;
                FastLED.show();
                vTaskDelay(onTime / portTICK_PERIOD_MS);
                this->mode = OFF;
                break;
            case OFF:
                leds[led] = CRGB::Black;
                FastLED.show();
                vTaskDelay(500 / portTICK_PERIOD_MS);
                break;
            default:
                FastLED.show();
                vTaskDelay(500 / portTICK_PERIOD_MS);
                break;
        }
    }
}

void LEDTask::setFade(uint8_t color, uint16_t fadeInTime, uint16_t fadeOutTime) {
    this->mode = FADE;
    this->color = color;
    this->onTime = fadeInTime;
    this->offTime = fadeOutTime;
}

void LEDTask::setFlicker(int color, uint16_t onTime, uint16_t offTime) {
    this->mode = FLICKER;
    this->color = color;
    this->onTime = onTime;
    this->offTime = offTime;
}

void LEDTask::setOffAfter(int color, uint16_t onTime) {
    this->mode = OFF_AFTER;
    this->color = color;
    this->onTime = onTime;
}

void LEDTask::turnOn(int color) {
    this->mode = ON;
    this->color = color;
}

void LEDTask::turnOff() {
    this->mode = OFF;
}
