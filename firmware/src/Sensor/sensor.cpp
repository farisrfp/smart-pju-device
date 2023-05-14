#include "sensor.h"

#include "../boards.h"

void cSensor::begin() {
    rtc.begin();
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

        printData();
    }
}

// Get Device AC Voltage
uint16_t
cSensor::getVoltage(void) {
    const uint16_t voltage = random(21000, 23000);

    return voltage;
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
    const uint32_t unixtime = rtc.now().unixtime();
    // const uint32_t unixtime = 1619712000;

    return unixtime;
}

// Get temperature (Stub)
float cSensor::getTemperature(void) {
    const float temperature = rtc.getTemperature();
    // const float temperature = 25.00;

    return temperature;
}

// Print Sensor Data
void cSensor::printData(void) {
    DEBUG_PRINTF("Temp = %.1f°C\t| Time = %d\t| Voltage = %dV\t| Current = %dmA\n", temperature_deg_c,
                 unix_time, voltage_v / 100, current_m_a);
}