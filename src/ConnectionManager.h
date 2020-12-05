#ifndef _CONNECTION_MANAGER_H_
#define _CONNECTION_MANAGER_H_

class MessageHandler {
public:
    virtual bool handleMessage(const char *message) = 0;

private:
    friend class ConnectionManager;
    MessageHandler *next;
};

class ConnectionManager {
public:
    ConnectionManager(const char *url);

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
    MessageHandler *handler;
    long lastUpdate;
    long lastConnect;
};

#endif
