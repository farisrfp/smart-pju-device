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
    for (;;) {
        switch (mode) {
            case FADE:
                for (int j = 0; j <= brightness; j++) {
                    switch (color) {
                        case RED:
                            leds[led] = CRGB(j, 0, 0);
                            break;
                        case GREEN:
                            leds[led] = CRGB(0, j, 0);
                            break;
                        case BLUE:
                            leds[led] = CRGB(0, 0, j);
                            break;
                    }
                    vTaskDelay((onTime / brightness) / portTICK_PERIOD_MS);
                }
                for (int j = brightness; j >= 0; j--) {
                    switch (color) {
                        case RED:
                            leds[led] = CRGB(j, 0, 0);
                            break;
                        case GREEN:
                            leds[led] = CRGB(0, j, 0);
                            break;
                        case BLUE:
                            leds[led] = CRGB(0, 0, j);
                            break;
                    }
                    vTaskDelay((offTime / brightness) / portTICK_PERIOD_MS);
                }
                break;
            case FLICKER:
                switch (color) {
                    case RED:
                        leds[led] = CRGB(255, 0, 0);
                        break;
                    case GREEN:
                        leds[led] = CRGB(0, 255, 0);
                        break;
                    case BLUE:
                        leds[led] = CRGB(0, 0, 255);
                        break;
                }
                vTaskDelay(onTime / portTICK_PERIOD_MS);
                leds[led] = CRGB(0, 0, 0);
                vTaskDelay(offTime / portTICK_PERIOD_MS);
                break;
            case ON:
                switch (color) {
                    case RED:
                        leds[led] = CRGB(255, 0, 0);
                        break;
                    case GREEN:
                        leds[led] = CRGB(0, 255, 0);
                        break;
                    case BLUE:
                        leds[led] = CRGB(0, 0, 255);
                        break;
                }
                vTaskDelay(500 / portTICK_PERIOD_MS);
                break;

            case OFF_AFTER:
                switch (color) {
                    case RED:
                        leds[led] = CRGB(255, 0, 0);
                        break;
                    case GREEN:
                        leds[led] = CRGB(0, 255, 0);
                        break;
                    case BLUE:
                        leds[led] = CRGB(0, 0, 255);
                        break;
                }
                vTaskDelay(onTime / portTICK_PERIOD_MS);
                this->mode = OFF;
                break;

            case OFF:
                leds[led] = CRGB::Black;
                vTaskDelay(500 / portTICK_PERIOD_MS);
                break;

            default:
                vTaskDelay(500 / portTICK_PERIOD_MS);
                break;
        }
    }
}

void LEDTask::setFade(uint8_t color, uint16_t fadeInTime, uint16_t fadeOutTime, uint8_t brightness) {
    this->mode = FADE;
    this->color = color;
    this->onTime = fadeInTime;
    this->offTime = fadeOutTime;
    this->brightness = brightness;
}

void LEDTask::setFlicker(uint8_t color, uint16_t onTime, uint16_t offTime) {
    this->mode = FLICKER;
    this->color = color;
    this->onTime = onTime;
    this->offTime = offTime;
}

void LEDTask::setOffAfter(uint8_t color, uint16_t onTime) {
    this->mode = OFF_AFTER;
    this->color = color;
    this->onTime = onTime;
}

void LEDTask::turnOn(uint8_t color) {
    this->mode = ON;
    this->color = color;
}

void LEDTask::turnOff() {
    this->mode = OFF;
}
