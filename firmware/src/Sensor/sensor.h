#pragma once

#include "../boards.h"

// LoRa_Serialization
#include <ACS712.h>
#include <LoraMessage.h>

extern RTC_DS3231 rtc;
extern ACS712 acs712;
extern ZMPT101B zmpt101b;

/****************************************************************************\
|
|	The sensor object
|
\****************************************************************************/

class cSensor {
   public:
    /**
     * @brief Construct a new cSensor object
     *
     */
    cSensor(){};

    // Sensor data function stubs
    float getTemperature(void);
    uint8_t getVoltage(void);
    uint16_t getCurrent(void);
    uint32_t getUnixTime(void);
    uint8_t getLightLevel(void);

    // turn PJU on/off
    void updateStatus();

    // update PJU dimmer
    void updateDimmer();

    /**
     * @brief Initialize the sensor
     */
    void begin();

    /**
     * @brief Update the sensor data
     */
    void loop();

    /**
     * @brief Print the sensor data
     */
    void printData();

    // Sensor status flags
    bool data;
    bool status = true;
    uint8_t dimmer = 0;

    // Sensor data
    float temperature_deg_c;  // outdoor air temperature in °C
    uint8_t voltage_v;        // battery voltage
    uint16_t current_m_a;     // supply voltage
    uint32_t unix_time;       // unix time
    uint8_t light_level;      // light level

   private:
    // void doUplink();

    // bool m_fUplinkRequest;           // set true when uplink is requested
    // bool m_fBusy;                    // set true while sending an uplink
    uint32_t m_lastUpdate;  // millis() timestamp of last update
};

// The global sensor instance
extern cSensor mySensor;
