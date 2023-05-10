#include "sensor.h"

#include "../boards.h"

// Sensor Variables
RTC_DS3231 rtc;

void cSensor::begin() {
    // rtc.begin();
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
    }
}

//
// Get Device AC Voltage
//
uint16_t
cSensor::getVoltage(void) {
    const uint16_t voltage = 3850;

    DEBUG_PRINTF("Device VOLTAGE = %dmV\n", voltage);

    return voltage;
}

//
// Get Device AC Current
//
uint16_t
cSensor::getCurrent(void) {
    const uint16_t voltage = 3300;

    DEBUG_PRINTF("Device CURRENT = %dmV\n", voltage);

    return voltage;
}

uint32_t
cSensor::getUnixTime(void) {
    // const uint32_t unixtime = rtc.now().unixtime();
    const uint32_t unixtime = 1619712000;

    // convert to date and time
    DEBUG_PRINTF("Device TIME = %s\n", rtc.now().timestamp());

    return unixtime;
}

//
// Get temperature (Stub)
//
float cSensor::getTemperature(void) {
    // const float temperature = rtc.getTemperature();
    const float temperature = 25.0;

    DEBUG_PRINTF("Device Environemt Temperature = %.1f°C\n", temperature);

    return temperature;
}