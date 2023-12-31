#include "ConnectionManager.h"
#include "LogManager.h"
#include "Variables.h"

#include <ESP8266WiFi.h>
#include <ESP8266WiFiGeneric.h>
#include <PubSubClient.h>

#define MAX_MSG_SIZE 1024

#ifdef MQTT_BROKER_HOST
#define MQTT_BROKER_HOST STR(MQTT_BROKER_HOST)
#else
#define MQTT_BROKER_HOST "guidos-macbook.local"
#endif

#ifndef MQTT_BROKER_PORT
#define MQTT_BROKER_PORT 1883
#endif

#ifdef MQTT_TOPIC_PREFIX
#define MQTT_TOPIC_PREFIX STR(MQTT_TOPIC_PREFIX)
#else
#define MQTT_TOPIC_PREFIX "guidowb.online"
#endif

const char *mqtt_broker_host  = MQTT_BROKER_HOST;
const int   mqtt_broker_port  = MQTT_BROKER_PORT;
const char *mqtt_topic_prefix = MQTT_TOPIC_PREFIX;
const char *mqtt_client_id    = STR(MQTT_CLIENT_ID);

static WiFiClient wifiClient;
static PubSubClient mqttClient(wifiClient);
static ConnectionManager *manager;

static Logger logger("connection");

static void callback(const char *topic, const uint8_t *message, uint32_t length) {
    manager->dispatchMessage(topic, message, length);
}

ConnectionManager::ConnectionManager(WiFiManager *wifi) {
    this->wifi = wifi;
    this->subscriptions = NULL;
    this->state = 9999;
}

void ConnectionManager::setup() {
    manager = this;
    mqttClient.setServer(mqtt_broker_host, mqtt_broker_port);
    mqttClient.setCallback(callback);
    subscribe("ping", this);
    subscribe("ping/lightshow", this);
    subscribe("ping/lightshow/%c", this);
}

void ConnectionManager::connect() {
    int lastState = this->state;
    this->state = mqttClient.state();

    if (!wifi->isConnected()) return;
    if (this->state == 0) return;
    if (mqttClient.connected()) return;

    bool success = mqttClient.connect(mqtt_client_id);
    int newState = mqttClient.state();

    if (success && newState == 0) {
        logger.info("connected to %s:%d as client id %s", mqtt_broker_host, mqtt_broker_port, mqtt_client_id);

        Subscription *subscription = this->subscriptions;
        char topic[MAX_MSG_SIZE];
        while (subscription) {
            expandTopic(topic, subscription->topic);
            logger.info("subscribing to topic: %s", topic);
            mqttClient.subscribe(topic);
            subscription = subscription->next;
        }
    }
    else if (newState != lastState) {
        logger.info("connect failed, state %d", state);
        logger.info("host: %s", mqtt_broker_host);
        logger.info("port: %d", mqtt_broker_port);
        logger.info("client id: %s", mqtt_client_id);
    }
    this->state = newState;
}

void ConnectionManager::loop() {
    if (!mqttClient.loop()) connect();
}

bool ConnectionManager::handleMessage(const char *topic, const uint8_t *message, uint32_t length) {
    if (!strcmp(topic, "ping") || !strncmp(topic, "ping/", 5)) {
        send("pong/lightshow/%c", "");
        return true;
    }
    return false;
}

void ConnectionManager::dispatchMessage(const char *topic, const uint8_t *message, uint32_t length) {
    topic = topic + strlen(mqtt_topic_prefix) + 1;
    Subscription *subscription = this->subscriptions;
    while (subscription != NULL) {
        if (subscription->handler->handleMessage(topic, message, length)) return;
        subscription = subscription->next;
    }
    logger.error("No handler for topic %s", topic);
}

void ConnectionManager::subscribe(const char *topic, MessageHandler *handler) {
    Subscription *subscription = new Subscription;
    subscription->topic = topic;
    subscription->handler = handler;
    subscription->next = this->subscriptions;
    this->subscriptions = subscription;
}

void ConnectionManager::send(const char *topic, const char *fmt, ...) {
    if (this->state != 0) return;

    va_list args;
    va_start(args, fmt);
    char buffer[MAX_MSG_SIZE];
    char *message = expandTopic(buffer, topic);
    vsnprintf(message, MAX_MSG_SIZE + buffer - message, fmt, args);
    mqttClient.publish(buffer, message);
    va_end(args);
}

char *ConnectionManager::expandTopic(char *buffer, const char *topic) {
    char *tgt = buffer;
    const char *src = topic;
    tgt += sprintf(tgt, "%s/", MQTT_TOPIC_PREFIX);
    while (char c = *src++) {
        if (c == '%' && *src == 'c') {
            tgt += sprintf(tgt, "%s", mqtt_client_id);
            src++;
            continue;
        }
        *tgt++ = c;
    }
    *tgt++ = '\0';
    return tgt;
}
