#include "WifiManager.h"

#ifndef MEGA

#include <ESP8266WiFi.h>
#include <wifi.h>

void WiFiManager::setup() {
    WiFi.mode(WIFI_STA);
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
    WiFi.setAutoReconnect(true);
}

void WiFiManager::loop() {
    // if (WiFi.status() == WL_CONNECTED) {}
}

#endif