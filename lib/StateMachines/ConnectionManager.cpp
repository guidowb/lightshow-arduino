#include "ConnectionManager.h"
#include "LogManager.h"

#include <ArduinoWebsockets.h>

#define MAX_MSG_SIZE 1024

using namespace websockets;

static WebsocketsClient client;
static ConnectionManager *manager;

static Logger logger("connection");

void onMessageCallback(WebsocketsMessage message) {
    const char *str = message.c_str();
    logger.debug("Received message: %s", str);
    manager->handleMessage(str);
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if (event == WebsocketsEvent::ConnectionOpened) {
        logger.info("Connection opened");
        client.ping();
        manager->setConnected(true);
    }
    else if (event == WebsocketsEvent::ConnectionClosed) {
        logger.info("Connection closed");
        manager->setConnected(false);
    }
    else if (event == WebsocketsEvent::GotPing) {
        logger.debug("Received ping, sending pong");
        client.pong();
    }
    else if (event == WebsocketsEvent::GotPong) {
        logger.debug("Received pong");
    }
}

ConnectionManager::ConnectionManager(const char *url, const char *program) {
    this->url = url;
    this->program = program;
    this->connected = false;
}

void ConnectionManager::setup() {
    manager = this;
    client.addHeader("Origin", "lightshow-arduino");
    client.addHeader("Cookie", String("macAddress=") + WiFi.macAddress() + "; program=" + program);
    client.onEvent(onEventsCallback);
    client.onMessage(onMessageCallback);
    connect();
}

void ConnectionManager::connect() {
    long now = millis();
    if (now - lastConnect >= 5000) {
        lastConnect = now;
        if (client.connect(url)) {
            logger.info("Connect successful");
        }
        else {
            logger.info("Connect failed");
        }
    }
}

void ConnectionManager::loop() {
    if (!connected) connect();
    client.poll();
    long now = millis();
    if (now - lastUpdate >= 40000) {
        logger.debug("Sending ping");
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
    logger.error("No handler for message");
}

void ConnectionManager::add(MessageHandler *handler) {
    handler->next = this->handler;
    this->handler = handler;
}

void ConnectionManager::send(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[MAX_MSG_SIZE];
    vsnprintf(buffer, MAX_MSG_SIZE, fmt, args);
    client.send(buffer);
    lastUpdate = millis();
    va_end(args);
}