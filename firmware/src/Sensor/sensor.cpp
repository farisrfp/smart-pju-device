#include "sensor.h"

#include "../boards.h"

void cSensor::begin() {
#ifndef DUMMY_DATA
    rtc.begin();
    acs712.setZeroPoint(ACS712_ZP);
    zmpt101b.setZeroPoint(ZMPT101B_ZP);
#endif
}

void cSensor::loop() {
    auto const tNow = millis();
    auto const deltaT = tNow - this->m_lastUpdate;

    if (deltaT >= 1000) {
        this->m_lastUpdate = tNow;

        unix_time = getUnixTime();
        temperature_deg_c = getTemperature();
        voltage_v = getVoltage();
        current_m_a = getCurrent();
        light_level = getLightLevel();

        printData();
    }
}

// Get Device AC Voltage
float cSensor::getVoltage(void) {
#ifndef DUMMY_DATA
    const float voltage = zmpt101b.getVoltageAC();
#else
    const float voltage = random(220, 240);
#endif

    return voltage;
}

// Get Device AC Current
uint16_t
cSensor::getCurrent(void) {
#ifndef DUMMY_DATA
    const uint16_t current = acs712.getCurrentAC();
#else
    const uint16_t current = random(800, 1500);
#endif

    return current;
}

// Get Device Unix Time
uint32_t
cSensor::getUnixTime(void) {
#ifndef DUMMY_DATA
    const uint32_t unixtime = rtc.now().unixtime();
#else
    const uint32_t unixtime = 1684120499;
#endif

    return unixtime;
}

// Get temperature (Stub)
float cSensor::getTemperature(void) {
#ifndef DUMMY_DATA
    const float temperature = rtc.getTemperature();
#else
    const float temperature = random(20, 30);
#endif

    return temperature;
}

// Get Light Level
uint8_t cSensor::getLightLevel(void) {
    const uint8_t lightLevel = random(0, 100);

    return lightLevel;
}

// Print Sensor Data
void cSensor::printData(void) {
    DEBUG_PRINTF("Temp = %.1f°C\t| Time = %d\t| Voltage = %.2fV\t| Current = %dmA\n", temperature_deg_c,
                 unix_time, voltage_v, current_m_a);
    // print esp32 temp
    DEBUG_PRINTF("ESP32 CPU Temp = %.1f°C\n", temperatureRead());
    char dataBuff[256];
    sprintf(dataBuff, "{\"type\":\"message\",\"temperature\":%.1f,\"light\":%d,\"voltage\":%.2f,\"current\":%d,\"time\":%d}",
            temperature_deg_c, light_level, voltage_v, current_m_a, unix_time);

    if (wsClient != nullptr && wsClient->canSend()) {
        Serial.println("Sending data to client by websocket");
        wsClient->text(dataBuff);
    }
}