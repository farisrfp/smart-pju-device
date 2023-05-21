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
    /// \brief the constructor. Deliberately does very little.
    cSensor(){};

    // Sensor data function stubs
    float getTemperature(void);
    float getVoltage(void);
    uint16_t getCurrent(void);
    uint32_t getUnixTime(void);
    uint8_t getLightLevel(void);

    ///
    /// \brief set up the sensor object
    ///
    ///
    void begin();

    ///
    /// \brief update sensor loop.
    ///
    /// \details
    ///     This should be called from the global loop(); it periodically
    ///     gathers and transmits sensor data.
    ///
    void loop();

    ///
    /// \brief print sensor data to serial port
    ///
    void printData();

    // Example sensor status flags
    bool data;   //<! sensor data validation
    bool relay;  //<! actuator relay status

    // Example sensor data
    float temperature_deg_c;  //<! outdoor air temperature in °C
    float voltage_v;          //<! battery voltage
    uint16_t current_m_a;     //<! supply voltage
    uint32_t unix_time;       //<! unix time
    uint8_t light_level;      //<! light level

   private:
    // void doUplink();

    // bool m_fUplinkRequest;           // set true when uplink is requested
    // bool m_fBusy;                    // set true while sending an uplink
    uint32_t m_lastUpdate;  // millis() timestamp of last update
};

// The global sensor instance
extern cSensor mySensor;
