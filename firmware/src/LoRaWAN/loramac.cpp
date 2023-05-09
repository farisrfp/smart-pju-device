#include "loramac.h"

#include "../boards.h"
// APPEUI, DEVEUI and APPKEY
#include "secrets.h"

// The pin map
const cMyLoRaWAN::lmic_pinmap myPinMap = {
    .nss = PIN_NSS,
    .rxtx = cMyLoRaWAN::lmic_pinmap::LMIC_UNUSED_PIN,
    .rst = PIN_RST,
    .dio = {PIN_DIO0, PIN_DIO1, PIN_DIO2},
    .rxtx_rx_active = 0,
    .rssi_cal = 0,
    .spi_freq = 8000000,
};

/****************************************************************************\
|
|	LoRaWAN methods
|
\****************************************************************************/

// our setup routine does the class setup and then registers an event handler so
// we can see some interesting things
void cMyLoRaWAN::setup() {
    // simply call begin() w/o parameters, and the LMIC's built-in
    // configuration for this board will be used.
    bool res = this->Super::begin(myPinMap);
    DEBUG_PRINTF("Arduino_LoRaWAN::begin(): %d\n", res);

    //    LMIC_selectSubBand(0);
    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);

    this->RegisterListener(
        // use a lambda so we're "inside" the cMyLoRaWAN from public/private perspective
        [](void *pClientInfo, uint32_t event) -> void {
            auto const pThis = (cMyLoRaWAN *)pClientInfo;

            // for tx start, we quickly capture the channel and the RPS
            if (event == EV_TXSTART) {
                // use another lambda to make log prints easy
                myEventLog.logEvent(
                    (void *)pThis,
                    LMIC.txChnl,
                    LMIC.rps,
                    0,
                    // the print-out function
                    [](cEventLog::EventNode_t const *pEvent) -> void {
                        Serial.print(F(" TX:"));
                        myEventLog.printCh(std::uint8_t(pEvent->getData(0)));
                        myEventLog.printRps(rps_t(pEvent->getData(1)));
                    });
            }
            // else if (event == some other), record with print-out function
            else {
                // do nothing.
            }
        },
        (void *)this  // in case we need it.
    );
}

// this method is called when the LMIC needs OTAA info.
// return false to indicate "no provisioning", otherwise
// fill in the data and return true.
bool cMyLoRaWAN::GetOtaaProvisioningInfo(
    OtaaProvisioningInfo *pInfo) {
    // these are the same constants used in the LMIC compliance test script; eases testing
    // with the RedwoodComm RWC5020B/RWC5020M testers.

    // initialize info
    memcpy(pInfo->DevEUI, deveui, sizeof(pInfo->DevEUI));
    memcpy(pInfo->AppEUI, appeui, sizeof(pInfo->AppEUI));
    memcpy(pInfo->AppKey, appkey, sizeof(pInfo->AppKey));

    return true;
}

// This method is called after the node has joined the network.
void cMyLoRaWAN::NetJoin(
    void) {
    DEBUG_PRINTF_TS("NetJoin()\n");
    // sleepTimeout = os_getTime() + sec2osticks(SLEEP_TIMEOUT_JOINED);
}

// This method is called after transmission has been completed.
// If enabled, the controller goes into deep sleep mode now.
void cMyLoRaWAN::NetTxComplete(
    void) {
    DEBUG_PRINTF_TS("NetTxComplete()\n");
#ifdef SLEEP_EN
    DEBUG_PRINTF("Shutdown()\n");
    myLoRaWAN.Shutdown();
    ESP.deepSleep(SLEEP_INTERVAL * 1000000);
#endif
}

#ifdef _DEBUG_MODE_
// Print session info for debugging
void printSessionInfo(const cMyLoRaWAN::SessionInfo &Info) {
    Serial.printf("Tag:\t\t%d\n", Info.V1.Tag);
    Serial.printf("Size:\t\t%d\n", Info.V1.Size);
    Serial.printf("Rsv2:\t\t%d\n", Info.V1.Rsv2);
    Serial.printf("Rsv3:\t\t%d\n", Info.V1.Rsv3);
    Serial.printf("NetID:\t\t0x%08X\n", Info.V1.NetID);
    Serial.printf("DevAddr:\t0x%08X\n", Info.V1.DevAddr);
    Serial.printf("NwkSKey:\t");
    for (int i = 0; i < 15; i++) {
        Serial.printf("%02X ", Info.V1.NwkSKey[i]);
    }
    Serial.printf("\n");
    Serial.printf("AppSKey:\t");
    for (int i = 0; i < 15; i++) {
        Serial.printf("%02X ", Info.V1.AppSKey[i]);
    }
    Serial.printf("\n");
}

// Print session state for debugging
void printSessionState(const cMyLoRaWAN::SessionState &State) {
    Serial.printf("Tag:\t\t%d\n", State.V1.Tag);
    Serial.printf("Size:\t\t%d\n", State.V1.Size);
    Serial.printf("Region:\t\t%d\n", State.V1.Region);
    Serial.printf("LinkDR:\t\t%d\n", State.V1.LinkDR);
    Serial.printf("FCntUp:\t\t%d\n", State.V1.FCntUp);
    Serial.printf("FCntDown:\t%d\n", State.V1.FCntDown);
    Serial.printf("gpsTime:\t%d\n", State.V1.gpsTime);
    Serial.printf("globalAvail:\t%d\n", State.V1.globalAvail);
    Serial.printf("Rx2Frequency:\t%d\n", State.V1.Rx2Frequency);
    Serial.printf("PingFrequency:\t%d\n", State.V1.PingFrequency);
    Serial.printf("Country:\t%d\n", State.V1.Country);
    Serial.printf("LinkIntegrity:\t%d\n", State.V1.LinkIntegrity);
    // There is more in it...
}
#endif

// Save Info to ESP32's RTC RAM
// if not possible, just do nothing and make sure you return false
// from NetGetSessionState().
void cMyLoRaWAN::NetSaveSessionInfo(
    const SessionInfo &Info,
    const uint8_t *pExtraInfo,
    size_t nExtraInfo) {
    // if (nExtraInfo > EXTRA_INFO_MEM_SIZE)
    //     return;
    // rtcSavedSessionInfo = Info;
    // rtcSavedNExtraInfo = nExtraInfo;
    // memcpy(rtcSavedExtraInfo, pExtraInfo, nExtraInfo);
    // magicFlag2 = MAGIC2;
    DEBUG_PRINTF_TS("NetSaveSessionInfo()\n");
#ifdef _DEBUG_MODE_
    printSessionInfo(Info);
#endif
}

/// Return saved session info (keys) from ESP32's RTC RAM
///
/// if you have persistent storage, you should provide a function
/// that gets the saved session info from persistent storage, or
/// indicate that there isn't a valid saved session. Note that
/// the saved info is opaque to the higher level.
///
/// \return true if \p sessionInfo was filled in, false otherwise.
///
/// Note:
/// According to "Purpose of NetSaveSessionInfo #165"
/// (https://github.com/mcci-catena/arduino-lorawan/issues/165)
/// "GetSavedSessionInfo() is effectively useless and should probably be removed to avoid confusion."
/// sic!
#if false
bool 
cMyLoRaWAN::GetSavedSessionInfo(
                SessionInfo &sessionInfo,
                uint8_t *pExtraSessionInfo,
                size_t nExtraSessionInfo,
                size_t *pnExtraSessionActual
                ) {
    if (magicFlag2 != MAGIC2) {
        // if not provided, default zeros buf and returns false.
        memset(&sessionInfo, 0, sizeof(sessionInfo));
        if (pExtraSessionInfo) {
            memset(pExtraSessionInfo, 0, nExtraSessionInfo);
        }
        if (pnExtraSessionActual) {
            *pnExtraSessionActual = 0;
        }
        DEBUG_PRINTF_TS("GetSavedSessionInfo() - failed\n");
        return false;
    } else {
        sessionInfo = rtcSavedSessionInfo;
        if (pExtraSessionInfo) {
            memcpy(pExtraSessionInfo, rtcSavedExtraInfo, nExtraSessionInfo);
        }
        if (pnExtraSessionActual) {
            *pnExtraSessionActual = rtcSavedNExtraInfo;
        }
        DEBUG_PRINTF_TS("GetSavedSessionInfo() - o.k.\n");
#ifdef _DEBUG_MODE_
            printSessionInfo(sessionInfo);
#endif
        return true;
    }
}
#endif

// Save State in RTC RAM. Note that it's often the same;
// often only the frame counters change.
// [If not possible, just do nothing and make sure you return false
// from NetGetSessionState().]
void cMyLoRaWAN::NetSaveSessionState(const SessionState &State) {
    // rtcSavedSessionState = State;
    // magicFlag1 = MAGIC1;
    DEBUG_PRINTF_TS("NetSaveSessionState()\n");
#ifdef _DEBUG_MODE_
    printSessionState(State);
#endif
}

// Either fetch SessionState from somewhere and return true or...
// return false, which forces a re-join.
bool cMyLoRaWAN::NetGetSessionState(SessionState &State) {
//     if (magicFlag1 == MAGIC1) {
//         State = rtcSavedSessionState;
//         DEBUG_PRINTF_TS("NetGetSessionState() - o.k.\n");
// #ifdef _DEBUG_MODE_
//         printSessionState(State);
// #endif
//         return true;
//     } else {
//         DEBUG_PRINTF_TS("NetGetSessionState() - failed\n");
        return false;
//     }
}

// Get APB provisioning info - this is also used in OTAA after a succesful join.
// If it can be provided in OTAA mode after a restart, no re-join is needed.
bool cMyLoRaWAN::GetAbpProvisioningInfo(AbpProvisioningInfo *pAbpInfo) {
    SessionState state;

    // ApbInfo:
    // --------
    // uint8_t         NwkSKey[16];
    // uint8_t         AppSKey[16];
    // uint32_t        DevAddr;
    // uint32_t        NetID;
    // uint32_t        FCntUp;
    // uint32_t        FCntDown;

    // if ((magicFlag1 != MAGIC1) || (magicFlag2 != MAGIC2)) {
    //     return false;
    // }
    DEBUG_PRINTF_TS("GetAbpProvisioningInfo()\n");

    // pAbpInfo->DevAddr = rtcSavedSessionInfo.V2.DevAddr;
    // pAbpInfo->NetID = rtcSavedSessionInfo.V2.NetID;
    // memcpy(pAbpInfo->NwkSKey, rtcSavedSessionInfo.V2.NwkSKey, 16);
    // memcpy(pAbpInfo->AppSKey, rtcSavedSessionInfo.V2.AppSKey, 16);
    // NetGetSessionState(state);
    // pAbpInfo->FCntUp = state.V1.FCntUp;
    // pAbpInfo->FCntDown = state.V1.FCntDown;

#ifdef _DEBUG_MODE_
    Serial.printf("NwkSKey:\t");
    for (int i = 0; i < 15; i++) {
        Serial.printf("%02X ", pAbpInfo->NwkSKey[i]);
    }
    Serial.printf("\n");
    Serial.printf("AppSKey:\t");
    for (int i = 0; i < 15; i++) {
        Serial.printf("%02X ", pAbpInfo->AppSKey[i]);
    }
    Serial.printf("\n");
    Serial.printf("FCntUp: %d\n", state.V1.FCntUp);
#endif
    return true;
}