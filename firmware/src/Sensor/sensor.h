#pragma once

// LoRa_Serialization
#include <LoraMessage.h>

// Sensor Configuration
#define PAYLOAD_SIZE 11

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

    void uplinkRequest(void) {
        m_fUplinkRequest = true;
    };
    ///
    /// \brief set up the sensor object
    ///
    /// \param uplinkPeriodMs optional uplink interval. If not specified,
    ///         transmit every six minutes.
    ///
    void setup(std::uint32_t uplinkPeriodMs = 6 * 60 * 1000);

    ///
    /// \brief update sensor loop.
    ///
    /// \details
    ///     This should be called from the global loop(); it periodically
    ///     gathers and transmits sensor data.
    ///
    void loop();

    // Example sensor status flags
    bool data;   //<! sensor data validation
    bool relay;  //<! actuator relay status

    // Example sensor data
    float temperature_deg_c;  //<! outdoor air temperature in °C
    uint16_t voltage_v;       //<! battery voltage
    uint16_t current_m_a;     //<! supply voltage
    uint32_t unix_time;       //<! unix time

   private:
    void doUplink();

    bool m_fUplinkRequest;           // set true when uplink is requested
    bool m_fBusy;                    // set true while sending an uplink
    std::uint32_t m_uplinkPeriodMs;  // uplink period in milliseconds
    std::uint32_t m_tReference;      // time of last uplink
};

// The global sensor instance
extern cSensor mySensor;
