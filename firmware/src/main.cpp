#include <Arduino.h>

#include "boards.h"

void setup() {
    // Wait for serial to be ready
    Serial.begin(115200);
    delay(500);

#ifdef DEBUG_MODE
    // wait for serial to be ready
    while (!Serial) {
        delay(1);
    }
#endif

    // initialize the board.
    initBoard();
    DEBUG_PRINTF_TS("initBoard() - done\n");

    mySensor.begin();
    DEBUG_PRINTF_TS("mySensor.begin() - done\n");

    wsInit();
    DEBUG_PRINTF_TS("wsInit() - done\n");

    setupLMIC();
    DEBUG_PRINTF_TS("setupLMIC() - done\n");

    Serial.println("[XLKM#4 PJU] Device is Ready for Serial");
}

void loop() {
    FastLED.show();
    loopLMIC();
    mySensor.loop();
    dnsServer.processNextRequest();

    // vTaskDelay(1000);
}
