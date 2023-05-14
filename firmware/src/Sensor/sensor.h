#pragma once

#include "../boards.h"

// LoRa_Serialization
#include <LoraMessage.h>

extern RTC_DS3231 rtc;

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
    uint16_t getVoltage(void);
    uint16_t getCurrent(void);
    uint32_t getUnixTime(void);

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
    uint16_t voltage_v;       //<! battery voltage
    uint16_t current_m_a;     //<! supply voltage
    uint32_t unix_time;       //<! unix time

   private:
    // void doUplink();

    // bool m_fUplinkRequest;           // set true when uplink is requested
    // bool m_fBusy;                    // set true while sending an uplink
    uint32_t m_lastUpdate;  // millis() timestamp of last update
};

// The global sensor instance
extern cSensor mySensor;
