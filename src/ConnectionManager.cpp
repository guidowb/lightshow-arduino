#include "ConnectionManager.h"

#include <ArduinoWebsockets.h>

using namespace websockets;

static WebsocketsClient client;
static ConnectionManager *manager;

void onMessageCallback(WebsocketsMessage message) {
    Serial.println("Received message");
    const char *str = message.c_str();
    Serial.println(str);
    manager->handleMessage(str);
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if (event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connection opened");
        client.ping();
        manager->setConnected(true);
    }
    else if (event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connection closed");
        manager->setConnected(false);
    }
    else if (event == WebsocketsEvent::GotPing) {
        Serial.println("Received ping, sending pong");
        client.pong();
    }
    else if (event == WebsocketsEvent::GotPong) {
        Serial.println("Received pong");
    }
}

ConnectionManager::ConnectionManager(const char *url) {
    this->url = url;
    this->connected = false;
}

void ConnectionManager::setup() {
    manager = this;
    client.addHeader("Origin", "lightshow-arduino");
    client.addHeader("Cookie", String("macAddress=") + WiFi.macAddress());
    client.onEvent(onEventsCallback);
    client.onMessage(onMessageCallback);
    connect();
}

void ConnectionManager::connect() {
    long now = millis();
    if (now - lastConnect >= 5000) {
        lastConnect = now;
        if (client.connect(url)) {
            Serial.println("Connect successful");
        }
        else {
            Serial.println("Connect failed");
        }
    }
}

void ConnectionManager::loop() {
    if (!connected) connect();
    client.poll();
    long now = millis();
    if (now - lastUpdate >= 40000) {
        Serial.println("Sending ping");
        client.ping();
        lastUpdate = now;
    }
}

void ConnectionManager::setConnected(bool connected) {
    this->connected = connected;
}

void ConnectionManager::handleMessage(const char *message) {
    MessageHandler *handler = this->handler;
    while (handler != NULL) {
        if (handler->handleMessage(message)) return;
        handler = handler->next;
    }
    Serial.println("No handler for message");
}

void ConnectionManager::add(MessageHandler *handler) {
    handler->next = this->handler;
    this->handler = handler;
}

void ConnectionManager::send(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    vsprintf(buffer, fmt, args);
    Serial.printf("Sending message: %s\n", buffer);
    client.send(buffer);
    lastUpdate = millis();
    va_end(args);
}