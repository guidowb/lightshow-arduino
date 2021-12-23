#ifndef _CONNECTION_MANAGER_H_
#define _CONNECTION_MANAGER_H_

#include "MessageHandler.h"

class ConnectionManager {
public:
    ConnectionManager(const char *url, const char *program);

    void setup();
    void loop();

    void add(MessageHandler *handler);
    void send(const char *fmt, ...);
    void handleMessage(const char *message);
    void connect();
    void setConnected(bool connected);

private:
    bool connected;
    const char *url;
    const char *program;
    MessageHandler *handler;
    long lastUpdate;
    long lastConnect;
};

#endif
