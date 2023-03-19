#include "LoRaWAN/loramac.h"
#include "PJU.h"
#include "WiFI/wifipoint.h"

struct sensor sensor_data {
    .temperature = 25,
    .voltage = 220,
    .current = 500,
    .light = 40,
    .time = DateTime(2023, 03, 19, 12, 30, 00)
};

unsigned long lastSend_1 = 0;
unsigned long lastSend_2 = 0;

void setup() {
    initBoard();
    initWiFiPoint();
    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.println("[XLKM#4 PJU] Device is Ready for Serial");
    setupLMIC();
}

void loop() {
    loopLMIC();
    server.handleClient();

    // sensor_data.time = rtc.now();
    // sensor_data.temperature = rtc.getTemperature();
    // sensor_data.voltage = analogRead(ADC_PIN) * 0.0048828125;
    // sensor_data.current = sensor_data.voltage / 0.1;
    // sensor_data.light = analogRead(ADC_PIN) * 0.0048828125;

    unsigned long currentMillis = millis();
    if (currentMillis - lastSend_1 >= 5000) {
        lastSend_1 = currentMillis;

        char buffer[256];
        snprintf(buffer, sizeof(buffer), "[Sensor] %d-%d-%d | %d:%d:%d | %.2f °C | %.2f V | %d mA | %d %%",
                 sensor_data.time.year(),
                 sensor_data.time.month(),
                 sensor_data.time.day(),
                 sensor_data.time.hour(),
                 sensor_data.time.minute(),
                 sensor_data.time.second(),
                 sensor_data.temperature,
                 sensor_data.voltage,
                 sensor_data.current,
                 sensor_data.light);
        Serial.println(buffer);
    }

    if (currentMillis - lastSend_2 >= 2500) {
        lastSend_2 = currentMillis;

        digitalWrite(BOARD_LED, !digitalRead(BOARD_LED));
    }
}
