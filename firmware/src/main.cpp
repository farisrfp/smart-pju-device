#include <Arduino.h>

#include "boards.h"

// Global Object/Variable
cSensor mySensor;

void setup() {
    // Wait for serial to be ready
    Serial.begin(115200);
    delay(500);

#ifdef DEBUG_MODE
    // wait for serial to be ready
    while (!Serial)
        yield();
#endif

    // initialize the board.
    initBoard();
    DEBUG_PRINTF_TS("initBoard() - done\n");

    setupLMIC();
    DEBUG_PRINTF_TS("setupLMIC() - done\n");

    mySensor.begin();
    DEBUG_PRINTF_TS("mySensor.begin() - done\n");

    Serial.println("[XLKM#4 PJU] Device is Ready for Serial");
}

void loop() {
    loopLMIC();
    mySensor.loop();

}
