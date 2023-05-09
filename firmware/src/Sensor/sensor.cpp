#include "sensor.h"

#include "../LoRaWAN/loramac.h"
#include "../boards.h"

// Sensor Variables
// RTC_DS3231 rtc;

/****************************************************************************\
|
|	Sensor methods
|
\****************************************************************************/

void cSensor::setup(std::uint32_t uplinkPeriodMs) {
    // set the initial time.
    this->m_uplinkPeriodMs = uplinkPeriodMs;
    this->m_tReference = millis();

    // rtc.begin();
}

void cSensor::loop(void) {
    auto const tNow = millis();
    auto const deltaT = tNow - this->m_tReference;

    if (deltaT >= this->m_uplinkPeriodMs) {
        // request an uplink
        this->m_fUplinkRequest = true;

        // keep trigger time locked to uplinkPeriod
        auto const advance = deltaT / this->m_uplinkPeriodMs;
        this->m_tReference += advance * this->m_uplinkPeriodMs;
    }

    // if an uplink was requested, do it.
    if (this->m_fUplinkRequest) {
        this->m_fUplinkRequest = false;
        this->doUplink();
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

//
// Prepare uplink data for transmission
//
void cSensor::doUplink(void) {
    // if busy uplinking, just skip
    if (this->m_fBusy) {
        DEBUG_PRINTF_TS("doUplink(): busy\n");
        return;
    }
    // if LMIC is busy, just skip
    if (LMIC.opmode & (OP_POLL | OP_TXDATA | OP_TXRXPEND)) {
        DEBUG_PRINTF_TS("doUplink(): other operation in progress\n");
        return;
    }

    // Call sensor data function stubs
    temperature_deg_c = getTemperature();
    voltage_v = getVoltage();
    current_m_a = getCurrent();
    unix_time = getUnixTime();

    // Status flags (Examples)
    data = true;
    relay = true;

    DEBUG_PRINTF("--- Uplink Data ---\n");
    DEBUG_PRINTF("Unix Time: %lu\n", unix_time);
    DEBUG_PRINTF("Device Temperature:   % 3.1f °C\n", temperature_deg_c);
    DEBUG_PRINTF("Device Voltage:       % 4d mV\n", voltage_v);
    DEBUG_PRINTF("Device Current:       % 4d mA\n", current_m_a);
    DEBUG_PRINTF("Status:\n");
    DEBUG_PRINTF("    data_ok:        %d\n", data_ok);

    DEBUG_PRINTF("\n");

    // Uplink payload buffer
    static uint8_t loraData[PAYLOAD_SIZE];
    // Serialize data into byte array
    // NOTE:
    // For TTN MQTT integration, ttn_decoder.js must be adjusted accordingly
    LoraEncoder encoder(loraData);
    encoder.writeBitmap(false, false, false, false, false, false,
                        data,
                        relay);                   // 1 Byte
    encoder.writeUnixtime(unix_time);             // 4 Bytes
    encoder.writeTemperature(temperature_deg_c);  // 2 Bytes
    encoder.writeUint16(voltage_v);               // 2 Bytes
    encoder.writeUint16(current_m_a);             // 2 Bytes

    this->m_fBusy = true;

    if (!myLoRaWAN.SendBuffer(
            loraData, sizeof(loraData),
            // this is the completion function:
            [](void *pClientData, bool fSucccess) -> void {
                auto const pThis = (cSensor *)pClientData;
                pThis->m_fBusy = false;
            },
            (void *)this,
            /* confirmed */ true,
            /* port */ 1)) {
        // sending failed; callback has not been called and will not
        // be called. Reset busy flag.
        this->m_fBusy = false;
    }
}