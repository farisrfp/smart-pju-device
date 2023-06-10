#include <Arduino.h>

#include "boards.h"

void setup() {
    // Wait for serial to be ready
    Serial.begin(115200);
    delay(500);

#ifdef VERBOSE
    // wait for serial to be ready
    while (!Serial) {
        delay(1);
    }
#endif

    // initialize the board.
    initBoard();
    DEBUG_PRINTF_TS("[SETUP_1] initBoard() - done\n");

    mySensor.begin();
    DEBUG_PRINTF_TS("[SETUP_2] mySensor.begin() - done\n");

    wsInit();
    DEBUG_PRINTF_TS("[SETUP_3] wsInit() - done\n");

    setupLMIC();
    DEBUG_PRINTF_TS("[SETUP_4] setupLMIC() - done\n");

    Serial.println("[XLKM#4 PJU] Device is Ready for Serial");
}

void loop() {
    loopLMIC();
    mySensor.loop();
    dnsServer.processNextRequest();
}
