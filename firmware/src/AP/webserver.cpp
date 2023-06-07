#include "webserver.h"

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
#ifdef VERBOSE
        Serial.println("Serving file:  /index.html");
#endif
        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/index.html", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    // sensor data
    server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request) {
        // use <ArduinoJson.h> to create json data
        StaticJsonDocument<256> json;
        json["type"] = "message";
        json["voltage"] = mySensor.voltage_v;
        json["current"] = mySensor.current_m_a;
        json["temperature"] = mySensor.temperature_deg_c;
        json["light"] = mySensor.light_level;

        String jsonStr;
        serializeJson(json, jsonStr);
        request->send(200, "application/json", jsonStr);
    });

    File root = LittleFS.open("/");
    while (File file = root.openNextFile()) {
        String filename = "/" + String(file.name());
        server.on(filename.c_str(), HTTP_GET, [filename](AsyncWebServerRequest *request) {
#ifdef VERBOSE
            Serial.println("Serving file: " + filename);
#endif
            String contentType = filename.substring(filename.length() - 3);
            if (contentType == "tml" || contentType == "htm")
                contentType = "text/html";
            else if (contentType == "css")
                contentType = "text/css";
            else if (contentType == ".js")
                contentType = "text/javascript";
            else if (contentType == "son")
                contentType = "application/json";
            else if (contentType == "jpg" || contentType == "peg")
                contentType = "image/jpeg";
            else if (contentType == "png")
                contentType = "image/png";
            else if (contentType == "svg")
                contentType = "image/svg+xml";
            else if (contentType == "ttf")
                contentType = "application/x-font-truetype";
            else if (contentType == "otf")
                contentType = "application/x-font-opentype";
            else
                contentType = "text/plain";
            AsyncWebServerResponse *response = request->beginResponse(LittleFS, filename, contentType);
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        });
    }

    // Captive portal to keep the client
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
