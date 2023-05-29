#ifndef ESP8266React_h
#define ESP8266React_h

#include <Arduino.h>
#include <AsyncTCP.h>
#include <WWWData.h>
#include <WiFi.h>

#include "FeaturesService.h"

#ifndef CORS_ORIGIN
#define CORS_ORIGIN "*"
#endif

// class ESP8266React {
//    public:
//     // ESP8266React(AsyncWebServer* server);

//     // void begin();
//     // void loop();

//     //  private:
//     // FeaturesService _featureService;
// };

class ESP8266React {
   public:
    ESP8266React(AsyncWebServer* server);

    // void begin();
    // void loop();
   private:
    FeaturesService _featureService;
};

#endif
