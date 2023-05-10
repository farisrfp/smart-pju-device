#include "loramac.h"

// Fill in your AppEUI, DevEUI and AppKey
#include "secrets.h"

const lmic_pinmap lmic_pins = {
    .nss = RADIO_CS_PIN,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = RADIO_RST_PIN,
    .dio = {RADIO_DIO0_PIN, RADIO_DIO1_PIN, RADIO_BUSY_PIN},
};

static osjob_t sendjob;
static int spreadFactor = DR_SF7;
static int joinStatus = EV_JOINING;
static const unsigned TX_INTERVAL = 30;
static String lora_msg = "";

void os_getArtEui(u1_t *buf) {
    memcpy_P(buf, appeui, 8);
}

void os_getDevEui(u1_t *buf) {
    memcpy_P(buf, deveui, 8);
}

void os_getDevKey(u1_t *buf) {
    memcpy_P(buf, appkey, 16);
}

void do_send(osjob_t *j) {
    if (joinStatus == EV_JOINING) {
        DEBUG_PRINTF_TS("[LoRaWAN] Not joined yet");
        // Check if there is not a current TX/RX job running
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);

    } else if (LMIC.opmode & OP_TXRXPEND) {
        DEBUG_PRINTF_TS("[LoRaWAN] OP_TXRXPEND, not sending");
    } else {
        DEBUG_PRINTF_TS("[LoRaWAN] OP_TXRXPEND,sending ...");
        led1.setOffAfter(GREEN, 250);

        byte loraData[10];
        LoraEncoder encoder(loraData);
        // encoder.writeBitmap
        encoder.writeUnixtime(mySensor.unix_time);
        encoder.writeTemperature(mySensor.temperature_deg_c);
        encoder.writeUint16(mySensor.voltage_v);
        encoder.writeUint16(mySensor.current_m_a);

        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, loraData, sizeof(loraData) - 1, 0);
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
    }
}

void onEvent(ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch (ev) {
        case EV_TXCOMPLETE:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_TXCOMPLETE (includes waiting for RX windows)");

            if (LMIC.txrxFlags & TXRX_ACK) {
                DEBUG_PRINTF_TS("[LoRaWAN] Received ack");
                lora_msg = "Received ACK.";
            }

            lora_msg = "rssi:" + String(LMIC.rssi) + " snr: " + String(LMIC.snr);

            if (LMIC.dataLen) {
                // data received in rx slot after tx
                Serial.print(F("[LoRaWAN] Data Received: "));
                Serial.write(LMIC.frame + LMIC.dataBeg, LMIC.dataLen);
                DEBUG_PRINTF_TS("\n");
                Serial.println(LMIC.dataLen);
                DEBUG_PRINTF_TS("[LoRaWAN]  bytes of payload");
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_JOINING:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_JOINING: -> Joining...");
            led1.setFade(COLOR::BLUE, 750, 500, 255);
            lora_msg = "OTAA joining....";
            joinStatus = EV_JOINING;

            break;
        case EV_JOIN_FAILED:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_JOIN_FAILED: -> Joining failed");
            led1.setFade(COLOR::RED, 350, 150, 255);
            lora_msg = "OTAA Joining failed";

            break;
        case EV_JOINED:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_JOINED");
            led1.turnOff();
            lora_msg = "Joined!";
            joinStatus = EV_JOINED;

            delay(3);
            // Disable link check validation (automatically enabled
            // during join, but not supported by TTN at this time).
            LMIC_setLinkCheckMode(0);

            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            DEBUG_PRINTF_TS("[LoRaWAN] EV_RXCOMPLETE");
            break;
        case EV_LINK_DEAD:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_LINK_DEAD");
            break;
        case EV_LINK_ALIVE:
            DEBUG_PRINTF_TS("[LoRaWAN] EV_LINK_ALIVE");
            break;
        default:
            DEBUG_PRINTF_TS("[LoRaWAN] Unknown event");
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
