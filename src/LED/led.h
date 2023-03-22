#pragma once

#include <FastLED.h>
#include <PJU.h>

enum COLOR {
    RED,
    GREEN,
    BLUE
};

enum MODE {
    FADE,
    FLICKER,
    ON,
    OFF_AFTER,
    OFF
};

#define NUM_LEDS 2
#define BRIGHTNESS 255

extern CRGB leds[NUM_LEDS];
extern LEDTask led1;
extern LEDTask led2;

class LEDTask {
   public:
    /**
     * Konstruktor untuk kelas LEDTask.
     *
     * @param led nomor LED yang akan digunakan.
     */
    LEDTask(uint8_t);

    /**
     * @brief Fungsi untuk mengaktifkan komponen dengan mode fading.
     *
     * @param color warna yang akan digunakan.
     * @param fadeInTime Durasi (dalam milidetik) untuk menyala semakin terang.
     * @param fadeOutTime Durasi (dalam milidetik) untuk padam semakin redup.
     * @param brightness Nilai maksimum brightness yang diinginkan.
     */
    void setFade(uint8_t, uint16_t, uint16_t, uint8_t);

    /**
     * Fungsi untuk mengaktifkan komponen dengan mode flicker.
     *
     * @param color warna yang akan digunakan.
     * @param onTime Durasi (dalam milidetik) untuk menyala.
     * @param offTime Durasi (dalam milidetik) untuk padam.
     */
    void setFlicker(uint8_t, uint16_t, uint16_t);

    /**
     * @brief Fungsi untuk menyalakan komponen.
     *
     * @param color warna yang akan digunakan.
     */
    void turnOn(uint8_t);

    /**
     * @brief Fungsi untuk menyalakan dan mematikan komponen.
     *
     * @param color warna yang akan digunakan.
     * @param onTime Durasi (dalam milidetik) untuk menyala.
     */
    void setOffAfter(uint8_t, uint16_t);

    /**
     * @brief Fungsi untuk mematikan komponen.
     */
    void turnOff();

   private:
    uint8_t led;
    uint8_t mode = 3;
    uint8_t color;
    uint16_t onTime;
    uint16_t offTime;
    uint8_t brightness;

    void Task(void *pvParameters);
};
