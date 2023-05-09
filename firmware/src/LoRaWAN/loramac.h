#pragma once

#include <Arduino_LoRaWAN_EventLog.h>
#include <Arduino_LoRaWAN_network.h>
#include <arduino_lmic.h>

// // LoRaWAN Configuration
// #define ARDUINO_LMIC_CFG_NETWORK_CHIRPSTACK 1

/****************************************************************************\
|
|	The LoRaWAN object
|
\****************************************************************************/

class cMyLoRaWAN : public Arduino_LoRaWAN_network {
   public:
    cMyLoRaWAN(){};
    using Super = Arduino_LoRaWAN_network;
    void setup();

   protected:
    // you'll need to provide implementation for this.
    virtual bool GetOtaaProvisioningInfo(Arduino_LoRaWAN::OtaaProvisioningInfo *) override;

    // NetTxComplete() activates deep sleep mode (if enabled)
    virtual void NetTxComplete(void) override;

    // NetJoin() changes <sleepTimeout>
    virtual void NetJoin(void) override;

    // Used to store/load data to/from persistent (at least during deep sleep) memory
    virtual void NetSaveSessionInfo(const SessionInfo &Info, const uint8_t *pExtraInfo, size_t nExtraInfo) override;
    // Note: GetSavedSessionInfo provided in simple_sensor_bme280.ino is not used anywhere
    // virtual bool GetSavedSessionInfo(SessionInfo &Info, uint8_t*, size_t, size_t*) override;
    virtual void NetSaveSessionState(const SessionState &State) override;
    virtual bool NetGetSessionState(SessionState &State) override;
    virtual bool GetAbpProvisioningInfo(AbpProvisioningInfo *pAbpInfo) override;
};

// the global LoRaWAN instance.
extern cMyLoRaWAN myLoRaWAN;
extern Arduino_LoRaWAN::cEventLog myEventLog;
