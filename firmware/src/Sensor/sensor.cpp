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
    acs712.setZeroPoint(ACS712_ZP);
    zmpt101b.setSensitivity(VOLTAGE_SENSITIVITY);
    pinMode(ADC_LIGHT, INPUT);
}

void cSensor::loop() {
    auto const tNow = millis();
    auto const deltaT = tNow - this->m_lastUpdate;

    if (deltaT >= 5000) {
        this->m_lastUpdate = tNow;

        unix_time = getUnixTime();
        temperature_deg_c = getTemperature();
        voltage_v = getVoltage();
        current_m_a = getCurrent();
        light_level = getLightLevel();

        printData();
    }

    // Schedule on-off power
    if (datetime.second() % 2 == 0) {
        // Serial.printf("Minute Even : %d\n", datetime.second());
        turnRelay(true);
    } else {
        // Serial.printf("Minute Odd : %d\n", datetime.second());
        turnRelay(false);
    }
}

// Get Device AC Voltage
float cSensor::getVoltage(void) {
#ifdef DUMMY_DATA
    const float voltage = random(220, 240);
#else
    const float voltage = zmpt101b.getRmsVoltage();
#endif

    return voltage;
}

// Get Device AC Current
uint16_t
cSensor::getCurrent(void) {
#ifdef DUMMY_DATA
    const uint16_t current = random(800, 1500);
#else
    const uint16_t current = acs712.getCurrentAC();
#endif

    return current;
}

// Get Device Unix Time
uint32_t
cSensor::getUnixTime(void) {
#ifdef DUMMY_DATA
    const uint32_t unixtime = 1684120499;
#else
    // Get current time
    datetime = rtc.now();
    const uint32_t unixtime = datetime.unixtime();
#endif

    return unixtime;
}

// Get temperature (Stub)
float cSensor::getTemperature(void) {
#ifdef DUMMY_DATA
    const float temperature = random(20, 30);
#else
    const float temperature = rtc.getTemperature();
#endif

    return temperature;
}

// Get Light Level
uint8_t cSensor::getLightLevel(void) {
#ifdef DUMMY_DATA
    const uint8_t lightLevel = random(0, 100);
#else
    const uint8_t lightLevel = analogRead(ADC_LIGHT);
    light_level = map(lightLevel, 0, 4023, 100, 0);
#endif

    return lightLevel;
}

// Turn on relay
bool cSensor::turnRelay(bool state) {
    if (state) {
        if (mySensor.relay) {
            return false;
        } else {
            Serial.println("Turn on relay");
#ifdef PROTOTYPE
            digitalWrite(LED_BOARD, HIGH);
#else
            digitalWrite(RELAY_PIN, LOW);
#endif
            mySensor.relay = true;
            return true;
        }

    } else {
        if (mySensor.relay) {
            Serial.println("Turn off relay");
#ifdef PROTOTYPE
            digitalWrite(LED_BOARD, LOW);
#else
            digitalWrite(RELAY_PIN, HIGH);
#endif
            mySensor.relay = false;

            return true;
        } else {
            return false;
        }
    }
}

// Print Sensor Data
void cSensor::printData(void) {
    DEBUG_PRINTF("Temp = %.1f°C\t| Time = %d\t| Voltage = %.2fV\t| Current = %dmA| Light = %d%%\n",
                 temperature_deg_c, unix_time, voltage_v, current_m_a, light_level);
    // print esp32 temp
    DEBUG_PRINTF("ESP32 CPU Temp = %.1f°C\n", temperatureRead());
    // Data to websocket
    char dataBuff[256];
    sprintf(dataBuff, "{\"type\":\"message\",\"temperature\":%.1f,\"light\":%d,\"voltage\":%.2f,\"current\":%d,\"rtc\":%d}",
            temperature_deg_c, light_level, voltage_v, current_m_a, unix_time);

    if (wsClient != nullptr && wsClient->canSend()) {
        Serial.println("Sending data to client by websocket");
        wsClient->text(dataBuff);
    }
}