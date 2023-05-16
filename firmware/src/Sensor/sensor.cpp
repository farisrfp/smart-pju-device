#include "sensor.h"

#include "../boards.h"

void cSensor::begin() {
#ifndef DUMMY_DATA
    rtc.begin();
#endif
}

void cSensor::loop(void) {
    auto const tNow = millis();
    auto const deltaT = tNow - this->m_lastUpdate;

    if (deltaT >= 10000) {
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
    const uint16_t voltage = random(21000, 23000);

    return voltage / 100.00;
}

// Get Device AC Current
uint16_t
cSensor::getCurrent(void) {
    const uint16_t current = random(800, 1500);

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

// Get Light Level (Stub)
uint8_t cSensor::getLightLevel(void) {
    const uint8_t lightLevel = random(0, 100);

    return lightLevel;
}

// Print Sensor Data
void cSensor::printData(void) {
    DEBUG_PRINTF("Temp = %.1f°C\t| Time = %d\t| Voltage = %.2fV\t| Current = %dmA\n", temperature_deg_c,
                 unix_time, voltage_v, current_m_a);
}