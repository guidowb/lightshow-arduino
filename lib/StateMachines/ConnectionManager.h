#ifndef _CONNECTION_MANAGER_H_
#define _CONNECTION_MANAGER_H_

#include "MessageHandler.h"
#include "WiFiManager.h"
#include "Arduino.h"

class ConnectionManager : public MessageHandler {
public:
    ConnectionManager(WiFiManager *wifi);

    void setup();
    void loop();
    void connect();

    // The topic arguments to subscribe and send is without the prefix, may contain any mqtt wildcards,
    // and may contain the special sequence %c to represent the client id. The same notation is supported
    // by MessageHandler::match()

    void subscribe(const char *topic, MessageHandler *handler);
    void send(const char *topic, const char *fmt, ...);
    void dispatchMessage(const char *topic, const uint8_t *message, uint32_t length);
    char *expandTopic(char *buffer, const char *topic);
    virtual bool handleMessage(const char *topic, const uint8_t *message, uint32_t length);

private:
    class Subscription {
    public:
        const char *topic;
        MessageHandler *handler;
        Subscription *next;
    };

    WiFiManager *wifi;
    Subscription *subscriptions;
};

#endif
