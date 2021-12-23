#ifndef _MESSAGEHANDLER_H_
#define _MESSAGEHANDLER_H_

class MessageHandler {
public:
    virtual bool handleMessage(const char *message) = 0;

protected:
    bool startsWith(const char *message, const char *word);

private:
    friend class ConnectionManager;
    MessageHandler *next;
};

#endif
