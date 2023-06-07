#pragma once

#include <Arduino.h>
// #include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>

#define DNS_PORT 53
#define SSID "Smart PJU XLKM#4"  // This is the SSID that ESP32 will broadcast

extern DNSServer dnsServer;
extern AsyncWebSocketClient *wsClient;


void wsInit(void);
void wsEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);