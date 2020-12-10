#include "WifiManager.h"

#ifndef MEGA

#include <ESP8266WiFi.h>
#include <ESP8266WiFiGeneric.h>

#include <wifi.h> // Secrets that live outside the repo

WiFiManager::WiFiManager() : connectionCheck(100), connectionAttempt(5000) {

}

void WiFiManager::setup() {
    WiFi.mode(WIFI_STA);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.setAutoReconnect(false);
    setState(WIFI_CONNECTING);
}

void WiFiManager::loop() {
    switch (getState()) {
    case WIFI_DISCONNECTED: tryConnecting(); break;
    case WIFI_CONNECTING:   checkConnected(); break;
    case WIFI_CONNECTED:    checkConnection(); break;
    }
}

void WiFiManager::tryConnecting() {
    if (!connectionAttempt.due()) return;
    Serial.println("WIFI trying to Connect ...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    setState(WIFI_CONNECTING);
}

void WiFiManager::checkConnected() {
    if (!connectionCheck.due()) return;
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("WIFI connected to %s\n", WiFi.SSID().c_str());
        setState(WIFI_CONNECTED);
        return;
    }
    if (timeInState(30000)) {
        Serial.println("WIFI failed to connect");
        connectionAttempt.reset();
        setState(WIFI_DISCONNECTED);
        return;
    }
}

void WiFiManager::checkConnection() {
    if (!connectionCheck.due()) return;
    if (WiFi.status() == WL_CONNECTED) return;
    Serial.println("WIFI disconnected");
    setState(WIFI_DISCONNECTED);
}

bool WiFiManager::isConnected() {
    return getState() == WIFI_CONNECTED;
}

#endif