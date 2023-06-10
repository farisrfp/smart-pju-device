#include "sensor.h"

#include "../boards.h"

TwoWire I2C_RTC = TwoWire(0);
DateTime datetime;

void cSensor::begin() {
#ifdef DUMMY_DATA
    return;
#endif
    I2C_RTC.begin(I2C_SDA, I2C_SCL, 100000);
    rtc.begin(&I2C_RTC);

    preferences.begin("sensor", false);

    uint16_t currentZP = preferences.getUShort("CURRENT", ACS712_ZP);
    uint16_t voltageSensitivity = preferences.getUShort("VOLTAGE", VOLTAGE_SENSITIVITY);

    acs712.setZeroPoint(currentZP);
    zmpt101b.setSensitivity(voltageSensitivity);
    pinMode(ADC_LIGHT, INPUT);
}

void cSensor::loop() {
    auto const tNow = millis();
    auto const deltaT = tNow - this->m_lastUpdate;
    static bool prevStatus;
    static uint8_t prevDimmer;

    // Check if data is not same as previous
    if (prevDimmer != dimmer) updateDimmer();
    prevDimmer = dimmer;

    if (prevStatus != status) updateStatus();
    prevStatus = status;

    if (deltaT >= 5000) {
        this->m_lastUpdate = tNow;
        led2.setOffAfter(CRGB::Orange, 500);

        unix_time = getUnixTime();
        temperature_deg_c = getTemperature();
        voltage_v = getVoltage();
        current_m_a = getCurrent();
        light_level = getLightLevel();

        printData();
    }

    // // Schedule on-off power
    // if (datetime.second() % 2 == 0) {
    //     // Serial.printf("Minute Even : %d\n", datetime.second());
    //     turnRelay(true);
    // } else {
    //     // Serial.printf("Minute Odd : %d\n", datetime.second());
    //     turnRelay(false);
    // }
}

// Get Device AC Voltage
uint8_t cSensor::getVoltage(void) {
#ifdef DUMMY_DATA
    const uint8_t voltage = random(215, 225);
#else
    const uint8_t voltage = zmpt101b.getRmsVoltage();
#endif

    return voltage;
}

// Get Device AC Current
uint16_t
cSensor::getCurrent(void) {
#ifdef DUMMY_DATA
    const uint16_t current = random(800, 1500);
#else
    const uint16_t current = acs712.getCurrentAC() * 1000;
#endif

    return current;
}

// Get Device Unix Time
uint32_t
cSensor::getUnixTime(void) {
#ifdef DUMMY_DATA
    uint32_t unixtime = 1684120499;
#else
    // Get current time
    datetime = rtc.now();
    uint32_t unixtime = datetime.unixtime();
#endif

    return unixtime;
}

// Get temperature (Stub)
float cSensor::getTemperature(void) {
#ifdef DUMMY_DATA
    float temperature = random(20, 30);
#else
    float temperature = rtc.getTemperature();
#endif

    return temperature;
}

// Get Light Level
uint8_t cSensor::getLightLevel(void) {
#ifdef DUMMY_DATA
    uint8_t lightLevel = random(0, 100);
#else
    // Convert to percentage
    uint8_t lightLevel = map(analogRead(ADC_LIGHT), 0, 4095, 0, 100);
#endif

    return lightLevel;
}

// Turn PJU On/Off
void cSensor::updateStatus() {
    if (status)
        dimmer = 100;
    else
        dimmer = 0;

    uint8_t dim_8bit = dimmer == 100 ? 0 : 255;
    ledcWrite(0, dim_8bit);

    DEBUG_PRINTF("[SENSOR] Status Changed to %d\n", status);
}

// Update PJU Dimmer
void cSensor::updateDimmer() {
    if (!status) return;

    uint8_t dim_8bit = map(dimmer, 0, 100, 255, 0);
    ledcWrite(0, dim_8bit);

    DEBUG_PRINTF("[SENSOR] Dimmer Changed to %d\n", dimmer);
}

// Print Sensor Data
void cSensor::printData(void) {
    // Print Data Sensor
    DEBUG_PRINTF("[SENSOR] ESP32 Temp: %.1f°C | Time = %d | %.2f V | %d mA | %.1f°C | %d Lux | Status = %d | Dimmer = %d\n",
                 temperatureRead(), unix_time, voltage_v, current_m_a, light_level, temperature_deg_c, mySensor.status, mySensor.dimmer);
}