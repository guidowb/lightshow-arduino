#include "ConnectionManager.h"

#include <ArduinoWebsockets.h>

using namespace websockets;

static WebsocketsClient client;
static ConnectionManager *manager;

void onMessageCallback(WebsocketsMessage message) {
    manager->handleMessage(message.c_str());
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {}
    else if(event == WebsocketsEvent::ConnectionClosed) { Serial.println("Websocket closed"); }
    else if(event == WebsocketsEvent::GotPing) {}
    else if(event == WebsocketsEvent::GotPong) {}
}

ConnectionManager::ConnectionManager(const char *url) {
    this->url = url;
}

void ConnectionManager::setup() {
    manager = this;
    client.onEvent(onEventsCallback);
    client.onMessage(onMessageCallback);
    client.connect(url);
}

void ConnectionManager::loop() {
    client.poll();
}

void ConnectionManager::handleMessage(const char *message) {
    MessageHandler *handler = this->handler;
    while (handler != NULL) {
        if (handler->handleMessage(message)) return;
    }
    Serial.println("No handler for message");
}

void ConnectionManager::add(MessageHandler *handler) {
    handler->next = this->handler;
    this->handler = handler;
}
