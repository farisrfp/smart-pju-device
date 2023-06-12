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

    server.on("/setting/lorawan", HTTP_GET, [](AsyncWebServerRequest *request) {
        uint8_t appEui[8];
        uint8_t devEui[8];
        uint8_t appKey[16];

        os_getArtEui(appEui);

        os_getDevEui(devEui);

        os_getDevKey(appKey);

        StaticJsonDocument<256> jsonLoRa;
        jsonLoRa["type"] = "message";
        jsonLoRa["dev_eui"] = String(devEui[0], HEX) + String(devEui[1], HEX) + String(devEui[2], HEX) + String(devEui[3], HEX) + String(devEui[4], HEX) + String(devEui[5], HEX) + String(devEui[6], HEX) + String(devEui[7], HEX);
        jsonLoRa["app_eui"] = String(appEui[0], HEX) + String(appEui[1], HEX) + String(appEui[2], HEX) + String(appEui[3], HEX) + String(appEui[4], HEX) + String(appEui[5], HEX) + String(appEui[6], HEX) + String(appEui[7], HEX);
        jsonLoRa["app_key"] = String(appKey[0], HEX) + String(appKey[1], HEX) + String(appKey[2], HEX) + String(appKey[3], HEX) + String(appKey[4], HEX) + String(appKey[5], HEX) + String(appKey[6], HEX) + String(appKey[7], HEX) + String(appKey[8], HEX) + String(appKey[9], HEX) + String(appKey[10], HEX) + String(appKey[11], HEX) + String(appKey[12], HEX) + String(appKey[13], HEX) + String(appKey[14], HEX) + String(appKey[15], HEX);

        String jsonStr;
        serializeJson(jsonLoRa, jsonStr);

        request->send(200, "application/json", jsonStr);
    });

    server.on("/setting/lorawan", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("app_eui", true)) {
            String appEui = request->getParam("app_eui", true)->value();
            uint8_t appEuiBytes[8];
            for (int i = 0; i < 8; ++i) {
                appEuiBytes[i] = strtoul(appEui.substring(i * 2, i * 2 + 2).c_str(), NULL, 16);
            }
            EEPROM.writeBytes(ADDR_APPEUI, appEuiBytes, 8);
            DEBUG_PRINTF("[AP] appEui changed to %s\n", appEui.c_str());
        }

        if (request->hasParam("dev_eui", true)) {
            String devEui = request->getParam("dev_eui", true)->value();
            uint8_t devEuiBytes[8];
            for (int i = 0; i < 8; ++i) {
                devEuiBytes[i] = strtoul(devEui.substring(i * 2, i * 2 + 2).c_str(), NULL, 16);
            }
            EEPROM.writeBytes(ADDR_DEVEUI, devEuiBytes, 8);
            DEBUG_PRINTF("[AP] devEui changed to %s\n", devEui.c_str());
        }

        if (request->hasParam("app_key", true)) {
            String appKey = request->getParam("app_key", true)->value();
            uint8_t appKeyBytes[16];
            for (int i = 0; i < 16; ++i) {
                appKeyBytes[i] = strtoul(appKey.substring(i * 2, i * 2 + 2).c_str(), NULL, 16);
            }
            EEPROM.writeBytes(ADDR_APPKEY, appKeyBytes, 16);
            DEBUG_PRINTF("[AP] appKey changed to %s\n", appKey.c_str());
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
