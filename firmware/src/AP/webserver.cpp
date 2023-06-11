#include "webserver.h"

const IPAddress apIP(192, 168, 2, 1);
const IPAddress gateway(255, 255, 255, 0);

StaticJsonDocument<256> json_1;
StaticJsonDocument<256> json_2;

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket websocket("/pju/ws");
AsyncWebSocketClient *wsClient;

// initial device state
AsyncWebSocketClient *clients[16];

void wsInit(void) {
    WiFi.disconnect();    // added to start with the wifi off, avoid crashing
    WiFi.mode(WIFI_OFF);  // added to start with the wifi off, avoid crashing
    WiFi.mode(WIFI_AP);

    WiFi.softAP(SSID);

    WiFi.softAPConfig(apIP, apIP, gateway);
    dnsServer.start(DNS_PORT, "*", apIP);
    DEBUG_PRINTF("[AP] WiFi AP is now running\n");
    DEBUG_PRINTF("[AP] IP address: %s\n", WiFi.softAPIP().toString().c_str());

    if (!LittleFS.begin()) {
        DEBUG_PRINTF("[AP] An Error has occurred while mounting LittleFS\n");
        return;
    }

    // bind websocket to async web server
    websocket.onEvent(wsEventHandler);
    server.addHandler(&websocket);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        DEBUG_PRINTF("[AP] Serving index.html\n");

        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/index.html", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    // sensor data
    server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request) {
        // use <ArduinoJson.h> to create json data
        json_1["type"] = "message";
        json_1["voltage"] = mySensor.voltage_v;
        json_1["current"] = mySensor.current_m_a;
        json_1["temperature"] = mySensor.temperature_deg_c;
        json_1["light"] = mySensor.light_level;
        json_1["rtc"] = mySensor.unix_time;

        String jsonStr;
        serializeJson(json_1, jsonStr);
        request->send(200, "application/json", jsonStr);
    });

    server.on("/setting/sensor", HTTP_GET, [](AsyncWebServerRequest *request) {
        uint16_t currentZP = EEPROM.readUShort(ADDR_CURRENT);
        uint16_t voltageSensitivity = EEPROM.readUShort(ADDR_VOLTAGE);

        String jsonStr = "{\"type\":\"message\",\"volt_sens\":" + String(voltageSensitivity) + ",\"curr_sens\":" + String(currentZP) + "}";

        request->send(200, "application/json", jsonStr);
    });

    server.on(
        "/setting/sensor", HTTP_POST, [](AsyncWebServerRequest *request) {
            if (request->hasParam("volt_sens", true)) {
                uint16_t voltageSensitivity = request->getParam("volt_sens", true)->value().toInt();
                EEPROM.writeUShort(ADDR_VOLTAGE, voltageSensitivity);
                zmpt101b.setSensitivity(voltageSensitivity);
                DEBUG_PRINTF("[AP] voltage_sens changed to %d\n", voltageSensitivity);
            }

            if (request->hasParam("curr_sens", true)) {
                uint16_t currentZP = request->getParam("curr_sens", true)->value().toInt();
                EEPROM.writeUShort(ADDR_CURRENT, currentZP);
                acs712.setZeroPoint(currentZP);
                DEBUG_PRINTF("[AP] current_sens changed to %d\n", currentZP);
            }

            EEPROM.commit();

            request->send(200, "application/json", "{\"type\":\"message\",\"message\":\"OK\"}");
        });

    File root = LittleFS.open("/");
    while (File file = root.openNextFile()) {
        String filename = "/" + String(file.name());
        server.on(filename.c_str(), HTTP_GET, [filename](AsyncWebServerRequest *request) {
            DEBUG_PRINTF("[AP] Serving %s\n", filename.c_str());

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

    DEBUG_PRINTF("[AP] Webserver is now running\n");
};

void wsEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_DATA) {
        // save the response as newest device state
        StaticJsonDocument<256> recievedJson;
        // parse the recieved json data
        DeserializationError error = deserializeJson(recievedJson, (char *)data, len);
        if (error) {
            DEBUG_PRINTF("[AP] deserializeJson() failed: %s\n", error.f_str());
            return;
        }

        if (strcmp(recievedJson["type"], "dimmer") == 0) {
            int dim_100 = recievedJson["dimmer"];
            mySensor.dimmer = dim_100;
        }

        if (strcmp(recievedJson["type"], "status") == 0) {
            bool stats = recievedJson["status"];
            mySensor.status = !stats;
        }

        // send ACK
        json_2["type"] = "ack";
        json_2["dimmer"] = mySensor.dimmer;
        json_2["status"] = mySensor.status;
        String ack;
        serializeJson(json_2, ack);
        client->text(ack);
        // alert all other clients
        for (int i = 0; i < 16; ++i) {
            if (clients[i] != NULL && clients[i] != client) clients[i]->text(ack);
        }
    } else if (type == WS_EVT_CONNECT) {
        Serial.println("Websocket client connection received");

        wsClient = client;

        // send current state
        json_2["type"] = "ratain";
        json_2["dimmer"] = mySensor.dimmer;
        json_2["status"] = mySensor.status;

        String jsonStr;
        serializeJson(json_2, jsonStr);
        // ACK with current state
        client->text(jsonStr);
        // store connected client
        for (int i = 0; i < 16; ++i) {
            if (clients[i] == NULL) {
                clients[i] = client;
                break;
            }
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
