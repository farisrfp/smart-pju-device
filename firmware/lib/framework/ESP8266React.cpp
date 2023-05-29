#include <ESP8266React.h>

ESP8266React::ESP8266React(AsyncWebServer* server) : _featureService(server) {
    WWWData::registerRoutes(
        [server, this](const String& uri, const String& contentType, const uint8_t* content, size_t len) {
            ArRequestHandlerFunction requestHandler = [contentType, content, len](AsyncWebServerRequest* request) {
                AsyncWebServerResponse* response = request->beginResponse_P(200, contentType, content, len);
                response->addHeader("Content-Encoding", "gzip");
                request->send(response);
            };
            server->on(uri.c_str(), HTTP_GET, requestHandler);
            // Serving non matching get requests with "/index.html"
            // OPTIONS get a straight up 200 response
            if (uri.equals("/index.html")) {
                server->onNotFound([requestHandler](AsyncWebServerRequest* request) {
                    if (request->method() == HTTP_GET) {
                        requestHandler(request);
                    } else if (request->method() == HTTP_OPTIONS) {
                        request->send(200);
                    } else {
                        request->send(404);
                    }
                });
            }
        });
}
