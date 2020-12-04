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
    void handleMessage(const char *message);

private:
    const char *url;
    MessageHandler *handler;
};

#endif
