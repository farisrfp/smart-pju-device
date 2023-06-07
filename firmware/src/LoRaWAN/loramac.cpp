#include "loramac.h"

// Fill in your AppEUI, DevEUI and AppKey
#include "secrets.h"

const lmic_pinmap lmic_pins = {
    .nss = PIN_NSS,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = PIN_RST,
    .dio = {PIN_DIO0, PIN_DIO1, PIN_DIO2},
};

static osjob_t sendjob;
static int spreadFactor = DR_SF7;
static int joinStatus = EV_JOINING;
static const unsigned TX_INTERVAL = 15;

void os_getArtEui(u1_t *buf) {
    preferences.begin("lorawan", false);
    uint8_t artEui[8];

    if (preferences.getBytes("artEui", artEui, 8) != 0) {
        memcpy(buf, artEui, 8);
    } else {
        memcpy_P(buf, APPEUI, 8);
    }
    Serial.printf("appEui: %02X%02X%02X%02X%02X%02X%02X%02X\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6],
                  buf[7]);
}

void os_getDevEui(u1_t *buf) {
    preferences.begin("lorawan", false);
    uint8_t devEui[8];

    if (preferences.getBytes("devEui", devEui, 8)) {
        memcpy(buf, devEui, 8);
    } else {
        memcpy_P(buf, DEVEUI, 8);
    }
    Serial.printf("devEui: %02X%02X%02X%02X%02X%02X%02X%02X\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6],
                  buf[7]);
}

void os_getDevKey(u1_t *buf) {
    preferences.begin("lorawan", false);
    uint8_t appKey[16];
    if (preferences.getBytes("appKey", appKey, 16) != 0) {
        memcpy(buf, appKey, 16);
    } else {
        memcpy_P(buf, APPKEY, 16);
    }
    Serial.printf(
        "appKey: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
        "%02X%02X%02X%02X%02X%02X\n",
        buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10],
        buf[11], buf[12], buf[13], buf[14], buf[15]);
}

void do_send(osjob_t *j) {
    if (joinStatus == EV_JOINING) {
        DEBUG_PRINTF_TS("[LoRaWAN] Not joined yet\n");
        // Check if there is not a current TX/RX job running
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);

    } else if (LMIC.opmode & OP_TXRXPEND) {
        DEBUG_PRINTF_TS("[LoRaWAN] OP_TXRXPEND, not sending\n");
    } else {
        DEBUG_PRINTF_TS("[LoRaWAN] OP_TXRXPEND,sending ...\n");
        led1.setOffAfter(COLOR::BLUE, 250);

        byte loraData[13];
        LoraEncoder encoder(loraData);
        encoder.writeUnixtime(mySensor.unix_time);
        encoder.writeTemperature(mySensor.temperature_deg_c);
        encoder.writeUint16(mySensor.voltage_v);
        encoder.writeUint16(mySensor.current_m_a);
        encoder.writeUint8(mySensor.light_level);
        encoder.writeUint8(mySensor.relay);
        encoder.writeUint8(mySensor.dimmer);

        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, loraData, sizeof(loraData), 0);
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
    }
}

void onEvent(ev_t ev) {
    switch (ev) {
        case EV_TXCOMPLETE:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_TXCOMPLETE, rssi: %d, snr: %d\n", LMIC.rssi, LMIC.snr);

            if (LMIC.txrxFlags & TXRX_ACK) {
                DEBUG_PRINTF_TS("[LoRaWAN] Received ack\n");
            }

            if (LMIC.dataLen) {
                // data received in rx slot after tx
                DEBUG_PRINTF_TS("[LoRaWAN] Data Received: %d bytes\n", LMIC.dataLen);

                if (LMIC.dataLen) {
                    for (int i = 0; i < LMIC.dataLen; i++) {
                        Serial.printf("%02X ", LMIC.frame[LMIC.dataBeg + i]);
                    }
                    Serial.println();

                    uint8_t method = LMIC.frame[LMIC.dataBeg + 0];
                    switch (method) {
                        int dim;
                        case 0x00:
                            DEBUG_PRINTF_TS("[LoRaWAN] Command: 0x00 -> On/Off\n");
                            if (mySensor.turnRelay(LMIC.frame[LMIC.dataBeg + 1])) Serial.println("Relay state changed");
                            break;
                        case 0x01:
                            DEBUG_PRINTF_TS("[LoRaWAN] Command: 0x01 -> Dimmer\n");
                            dim = map(LMIC.frame[LMIC.dataBeg + 1], 0, 100, 255, 0);
                            mySensor.dimmer = dim;
                            ledcWrite(0, mySensor.dimmer);
                            Serial.printf("Dimmer state changed to %d%\n", mySensor.dimmer);
                            break;
                        default:
                            break;
                    }
                }
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_JOINING:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_JOINING: -> Joining...\n");
            led1.setFade(COLOR::BLUE, 750, 500, 255);
            joinStatus = EV_JOINING;

            break;
        case EV_JOIN_FAILED:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_JOIN_FAILED: -> Joining failed\n");
            led1.setOffAfter(COLOR::RED, 1000);

            break;
        case EV_JOINED:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_JOINED\n");
            led1.setOffAfter(COLOR::GREEN, 1000);
            joinStatus = EV_JOINED;

            delay(3);
            // Disable link check validation (automatically enabled
            // during join, but not supported by TTN at this time).
            LMIC_setLinkCheckMode(0);

            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            DEBUG_PRINTF_TS("[LoRaWAN] EV_RXCOMPLETE\n");
            break;
        case EV_LINK_DEAD:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_LINK_DEAD\n");
            break;
        case EV_LINK_ALIVE:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_LINK_ALIVE\n");
            break;
        default:
            DEBUG_PRINTF_TS("[LoRaWAN] Unknown event\n");
            break;
    }
}

void setupLMIC(void) {
    // LMIC init
    os_init();

    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);
    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.

    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);   // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);  // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);   // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);   // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);   // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);   // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);   // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);   // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK, DR_FSK), BAND_MILLI);    // g2-band

    // TTN defines an additional channel at 869.525Mhz using SF9 for class B
    // devices' ping slots. LMIC does not have an easy way to define set this
    // frequency and support for class B is spotty and untested, so this
    // frequency is not configured here.

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(spreadFactor, 14);

    // Start job
    LMIC_startJoining();

    do_send(&sendjob);  // Will fire up also the join
}

void loopLMIC(void) {
    os_runloop_once();
}
