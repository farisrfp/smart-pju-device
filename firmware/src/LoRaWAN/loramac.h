#pragma once

#include <lmic.h>
#include <hal/hal.h>

#include "../boards.h"

// LoRa_Serialization
#include <LoraMessage.h>

void setupLMIC(void);
void loopLMIC(void);