#include "PJU.h"

ZMPT101B voltageSensor(ADC1_PIN);
ACS712 currentSensor(ACS712_30A, ADC2_PIN);

struct sensor sensor_data {
    .temperature = 25,
    .voltage = 220,
    .current = 500,
    .light = 40,
    .time = DateTime(2023, 03, 19, 12, 30, 00)
};

unsigned long lastSend_1 = 0;
unsigned long lastSend_2 = 0;
unsigned long lastSend_3 = 0;
 
void setup() {
    // initBoard();
    // initWiFiPoint();

    Serial.begin(115200);
    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);

    // pinMode(ADC1_PIN, INPUT);
    // pinMode(ADC2_PIN, INPUT);

    // voltageSensor.setVref(3.3);

    // voltageSensor.calibrate();
    // currentSensor.calibrate();

    Serial.println("[XLKM#4 PJU] Device is Ready for Serial");
    setupLMIC();
}

void loop() {
    loopLMIC();

    // server.handleClient();

    // sensor_data.time = rtc.now();
    // sensor_data.temperature = rtc.getTemperature();
    // sensor_data.voltage = voltageSensor.getVoltageAC();
    // sensor_data.current = currentSensor.getCurrentAC();
    // sensor_data.light = map(analogRead(ADC1_PIN), 0, 4000, 0, 100);

    unsigned long currentMillis = millis();
    if (currentMillis - lastSend_1 >= 2000) {
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

    if (currentMillis - lastSend_2 >= 25) {
        lastSend_2 = currentMillis;

        FastLED.show();
        // digitalWrite(BOARD_LED, !digitalRead(BOARD_LED));
    }

    if (currentMillis - lastSend_3 >= 10) {
        lastSend_3 = currentMillis;

        // // Zero point (Without power input)
        // int Z = voltageSensor.calibrate();

        // // Zero point (With power input)
        // int ZL = voltageSensor.calibrateLive();

        // // Zero point voltage (With or without power input)
        // float ZV = voltageSensor.calibrateVoltage();

        // if (Z <= (ZL + 3) && Z >= (ZL - 3)) {
        //     Serial.print(String("| ") + Z);
        // } else {
        //     Serial.print(String("| ---"));
        // }
        // Serial.print(String(" | ") + ZL);
        // Serial.println(String(" |") + ZV + "V|");
    }
}
