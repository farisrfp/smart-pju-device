#include <Arduino.h>
#include <ArduinoJson.h>
#include <lmic.h>
#include <hal/hal.h>

#include "PJU.h"

// Chose LSB mode on the console and then copy it here.
static const u1_t PROGMEM APPEUI[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// LSB mode
// {0xFC, 0x46, 0x74, 0x79, 0x3E, 0xE6, 0x82, 0xDA}
static const u1_t PROGMEM DEVEUI[8] = {0xDA, 0x82, 0xE6, 0x3E, 0x79, 0x74, 0x46, 0xFC};
// MSB mode
static const u1_t PROGMEM APPKEY[16] = {
    0x93, 0x14, 0xD0, 0xA3, 0xFC, 0xDE, 0x71,
    0x5B, 0xAF, 0x80, 0x73, 0x45, 0x2A, 0x09,
    0x1B, 0x52};

// Pin mapping
#ifdef STM32L073xx
const lmic_pinmap lmic_pins = {
    .nss = RADIO_CS_PIN,
    .rxtx = RADIO_SWITCH_PIN,
    .rst = RADIO_RST_PIN,
    .dio = {RADIO_DIO0_PIN, RADIO_DIO1_PIN, RADIO_DIO2_PIN},
    .rx_level = HIGH};
#else
const lmic_pinmap lmic_pins = {
    .nss = RADIO_CS_PIN,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = RADIO_RST_PIN,
    .dio = {RADIO_DIO0_PIN, RADIO_DIO1_PIN, RADIO_BUSY_PIN}};
#endif

static osjob_t sendjob;
static int spreadFactor = DR_SF7;
static int joinStatus = EV_JOINING;
static const unsigned TX_INTERVAL = 30;
static String lora_msg = "";

void os_getArtEui(u1_t *buf) {
    memcpy_P(buf, APPEUI, 8);
}

void os_getDevEui(u1_t *buf) {
    memcpy_P(buf, DEVEUI, 8);
}

void os_getDevKey(u1_t *buf) {
    memcpy_P(buf, APPKEY, 16);
}

void do_send(osjob_t *j) {
    if (joinStatus == EV_JOINING) {
        Serial.println(F("[LoRaWAN] Not joined yet"));
        // Check if there is not a current TX/RX job running
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);

    } else if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("[LoRaWAN] OP_TXRXPEND, not sending"));
    } else {
        Serial.println(F("[LoRaWAN] OP_TXRXPEND,sending ..."));
        // JSON
        DynamicJsonDocument doc(1024);
        doc["time"] = sensor_data.time.unixtime();
        doc["temperature"] = sensor_data.temperature;
        doc["voltage"] = sensor_data.voltage;
        doc["current"] = sensor_data.current;
        doc["light"] = sensor_data.light;

        String payload;
        serializeJson(doc, payload);

        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, (uint8_t *)payload.c_str(), payload.length(), 0);
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);

#ifdef HAS_DISPLAY
        char buf[256];
        snprintf(buf, sizeof(buf), "[%i:%i:%i] Data sending!", sensor_data.time.hour(), sensor_data.time.minute(), sensor_data.time.second());
        display.clearDisplay();
        display.setCursor(0, 12);
        display.println(buf);
        display.display();
#endif
    }
}

void onEvent(ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch (ev) {
        case EV_TXCOMPLETE:
            Serial.println(F("[LoRaWAN] EV_TXCOMPLETE (includes waiting for RX windows)"));

            if (LMIC.txrxFlags & TXRX_ACK) {
                Serial.println(F("[LoRaWAN] Received ack"));
                lora_msg = "Received ACK.";
            }

            lora_msg = "rssi:" + String(LMIC.rssi) + " snr: " + String(LMIC.snr);

            if (LMIC.dataLen) {
                // data received in rx slot after tx
                Serial.print(F("[LoRaWAN] Data Received: "));
                Serial.write(LMIC.frame + LMIC.dataBeg, LMIC.dataLen);
                Serial.println();
                Serial.println(LMIC.dataLen);
                Serial.println(F("[LoRaWAN]  bytes of payload"));
            }
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_JOINING:
            Serial.println(F("[LoRaWAN] EV_JOINING: -> Joining..."));
            lora_msg = "OTAA joining....";
            joinStatus = EV_JOINING;
#ifdef HAS_DISPLAY
            display.clearDisplay();
            display.setCursor(0, 12);
            display.println("OTAA joining....");
            display.display();
#endif
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("[LoRaWAN] EV_JOIN_FAILED: -> Joining failed"));
            lora_msg = "OTAA Joining failed";
#ifdef HAS_DISPLAY
            display.clearDisplay();
            display.setCursor(0, 12);
            display.println("OTAA Joining failed");
            display.display();
#endif
            break;
        case EV_JOINED:
            Serial.println(F("[LoRaWAN] EV_JOINED"));
            lora_msg = "Joined!";
            joinStatus = EV_JOINED;

#ifdef HAS_DISPLAY
            display.clearDisplay();
            display.setCursor(0, 12);
            display.println("Joined TTN!");
            display.display();
#endif
            delay(3);
            // Disable link check validation (automatically enabled
            // during join, but not supported by TTN at this time).
            LMIC_setLinkCheckMode(0);

            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("[LoRaWAN] EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("[LoRaWAN] EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("[LoRaWAN] EV_LINK_ALIVE"));
            break;
        default:
            Serial.println(F("[LoRaWAN] Unknown event"));
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
