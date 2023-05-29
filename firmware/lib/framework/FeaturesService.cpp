#include <FeaturesService.h>

FeaturesService::FeaturesService(AsyncWebServer* server) {
    server->on(FEATURES_SERVICE_PATH, HTTP_GET, std::bind(&FeaturesService::features, this, std::placeholders::_1));
}

void FeaturesService::features(AsyncWebServerRequest* request) {
    AsyncJsonResponse* response = new AsyncJsonResponse(false, MAX_FEATURES_SIZE);
    JsonObject root = response->getRoot();
    root["project"] = false;
    root["security"] = false;
    root["mqtt"] = false;
    root["ntp"] = false;
    root["ota"] = false;
    root["upload_firmware"] = false;
    response->setLength();
    request->send(response);
}
