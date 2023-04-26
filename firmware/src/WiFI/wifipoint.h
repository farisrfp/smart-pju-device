#pragma once

#include <WebServer.h>
#include <WiFi.h>

extern WebServer server;

String SendHTML();
void handle_OnConnect();
void handle_NotFound();
void initWiFiPoint();
