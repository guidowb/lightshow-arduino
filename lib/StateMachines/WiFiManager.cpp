#include "WifiManager.h"

#ifndef MEGA

#include <ESP8266WiFi.h>
#include <ESP8266WiFiGeneric.h>

#include <wifi.h> // Secrets that live outside the repo

WiFiManager::WiFiManager() : connectionCheck(100), connectionAttempt(5000) {}

void WiFiManager::setup() {
    WiFi.mode(WIFI_STA);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    setState(WIFI_CONNECTING);
}

void WiFiManager::loop() {
    switch (getState()) {
    case WIFI_DISCONNECTED: whileDisconnected(); break;
    case WIFI_CONNECTING:   whileConnecting();   break;
    case WIFI_CONNECTED:    whileConnected();    break;
    }
}

void WiFiManager::enterDisconnected() {
    setState(WIFI_DISCONNECTED);
    Serial.println("WIFI disconnected");
    connectionAttempt.reset();
}

void WiFiManager::whileDisconnected() {
    if (connectionCheck.due()) {
        if (WiFi.status() == WL_CONNECTED) {
            enterConnected();
            return;
        }
    }
    if (connectionAttempt.due()) {
        enterConnecting();
        return;
    }
}

void WiFiManager::enterConnecting() {
    setState(WIFI_CONNECTING);
    Serial.println("WIFI trying to connect ...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void WiFiManager::whileConnecting() {
    if (!connectionCheck.due()) return;
    if (WiFi.status() == WL_CONNECTED) {
        enterConnected();
        return;
    }
    if (timeInState(30000)) {
        enterConnecting();
        return;
    }
}

void WiFiManager::enterConnected() {
    Serial.printf("WIFI connected to %s\n", WiFi.SSID().c_str());
    setState(WIFI_CONNECTED);
}

void WiFiManager::whileConnected() {
    if (!connectionCheck.due()) return;
    if (WiFi.status() == WL_CONNECTED) return;
    enterDisconnected();
}

bool WiFiManager::isConnected() {
    return getState() == WIFI_CONNECTED;
}

#endif