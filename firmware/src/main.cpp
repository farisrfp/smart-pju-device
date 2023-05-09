#include <Arduino.h>

#include "boards.h"

// Global Object/Variable
Arduino_LoRaWAN::cEventLog myEventLog;
cMyLoRaWAN myLoRaWAN{};
cSensor mySensor{};

void setup() {
    // set baud rate
    Serial.begin(115200);

    // wait for serial to be ready
    while (!Serial)
        yield();
    delay(500);

    initBoard();
    DEBUG_PRINTF_TS("setup()\n");

    // set up the log; do this first.
    myEventLog.setup();
    DEBUG_PRINTF("myEventlog.setup() - done\n");

    // set up the sensors.
    mySensor.setup();
    DEBUG_PRINTF("mySensor.setup() - done\n");

    // set up lorawan.
    myLoRaWAN.setup();
    DEBUG_PRINTF("myLoRaWAN.setup() - done\n");

    mySensor.uplinkRequest();
    Serial.println("[XLKM#4 PJU] Device is Ready for Serial");
}

/****************************************************************************\
|
|	loop()
|
\****************************************************************************/

void loop() {
    // the order of these is arbitrary, but you must poll them all.
    myLoRaWAN.loop();
    mySensor.loop();
    myEventLog.loop();
    Serial.println("[XLKM#4 PJU] Device is Ready for Serial");
}
