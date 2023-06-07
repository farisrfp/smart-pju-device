#include "webserver.h"

#include "ESP8266React.h"

const IPAddress apIP(192, 168, 2, 1);
const IPAddress gateway(255, 255, 255, 0);

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");
AsyncWebSocketClient *wsClient;

void wsInit(void) {
    WiFi.disconnect();    // added to start with the wifi off, avoid crashing
    WiFi.mode(WIFI_OFF);  // added to start with the wifi off, avoid crashing
    WiFi.mode(WIFI_AP);

    WiFi.softAP(SSID);

    WiFi.softAPConfig(apIP, apIP, gateway);
    dnsServer.start(DNS_PORT, "*", apIP);
    Serial.println("");
    Serial.println("WiFi AP is now running");
    Serial.println("IP address: ");
    Serial.println(WiFi.softAPIP());

        if (!LittleFS.begin()) {
            Serial.println("An Error has occurred while mounting LittleFS");
            return;
        }

    // bind websocket to async web server
    websocket.onEvent(wsEventHandler);
    server.addHandler(&websocket);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("Serving file:  /index.html");
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", ESP_REACT_DATA_8, sizeof(ESP_REACT_DATA_8));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/css/179.3ade.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("Serving file:  /css/179.bb70.css");
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", ESP_REACT_DATA_9, sizeof(ESP_REACT_DATA_9));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/js/179.8527.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("Serving file:  /js/179.45b6.js");
        AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", ESP_REACT_DATA_6, sizeof(ESP_REACT_DATA_6));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server.on("/static/media/Group 20.a299069e3828ebb880fb.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("Serving file:  /static/media/Group 20.a299069e.png");
        AsyncWebServerResponse *response = request->beginResponse_P(200, "image/png", ESP_REACT_DATA_7, sizeof(ESP_REACT_DATA_7));
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    // // Captive portal to keep the client
    server.on("*", HTTP_GET, [](AsyncWebServerRequest *request) { request->redirect("http://" + apIP.toString()); });
    server.begin();

    Serial.println("Server Started");
};

// allocate memory for recieved json data
#define BUFFER_SIZE 512
StaticJsonDocument<BUFFER_SIZE> recievedJson;
// initial device state
char dataBuffer[BUFFER_SIZE] = "{\"type\":\"message\",\"temp\":25.0,\"light\":50,\"voltage\":3.3,\"current\":1000,\"rtc\":1111111111}";
AsyncWebSocketClient *clients[16];

void wsEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_DATA) {
        // save the response as newest device state
        for (int i = 0; i < len; ++i) dataBuffer[i] = data[i];
        dataBuffer[len] = '\0';
        Serial.println(dataBuffer);
        // parse the recieved json data
        DeserializationError error = deserializeJson(recievedJson, (char *)data, len);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }
        if (strcmp(recievedJson["type"], "message") != 0)
            return;
        // get the target LED state
        bool led = recievedJson["LED"];
        digitalWrite(2, led);
        // send ACK
        client->text(dataBuffer, len);
        // alert all other clients
        for (int i = 0; i < 16; ++i)
            if (clients[i] != NULL && clients[i] != client)
                clients[i]->text(dataBuffer, len);
    } else if (type == WS_EVT_CONNECT) {
        Serial.println("Websocket client connection received");

        wsClient = client;
        // ACK with current state
        client->text(dataBuffer);
        // store connected client
        for (int i = 0; i < 16; ++i)
            if (clients[i] == NULL) {
                clients[i] = client;
                break;
            }
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.println("Client disconnected");

        wsClient = nullptr;
        // remove client from storage
        for (int i = 0; i < 16; ++i)
            if (clients[i] == client)
                clients[i] = NULL;
    }
}
