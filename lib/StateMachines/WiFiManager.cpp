#include "WifiManager.h"

#ifndef MEGA

#include <ESP8266WiFi.h>
#include <wifi.h>
#include <ESP8266WiFiGeneric.h>

static void stationModeDisconnected(const WiFiEventStationModeDisconnected &event) {
    Serial.printf("Station mode disconnected, reason %d\n", event.reason);
}

void WiFiManager::setup() {
    WiFi.mode(WIFI_STA);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.onStationModeDisconnected(stationModeDisconnected);
    connect();
}

void WiFiManager::connect() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
    WiFi.setAutoReconnect(false); //                                              <--------------------- FIX THIS
    while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(1000); }
    Serial.println("\nConnected to WiFi");
    connected = true;
}

void WiFiManager::loop() {
    if (connected && WiFi.status() != WL_CONNECTED) {
        Serial.printf("Free Memory: %d\n", ESP.getFreeHeap());
        Serial.printf("WiFi disconnected, status %d\n", WiFi.status());
        connected = false;
    }
    if (!connected) {
        long now = millis();
        if (now - lastConnect >= 5000) {
            connect();
            lastConnect = now;
        }
    }
}

#endif