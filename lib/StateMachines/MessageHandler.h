#ifndef _MESSAGEHANDLER_H_
#define _MESSAGEHANDLER_H_

#include "Arduino.h"

class MessageHandler {
public:
    virtual bool handleMessage(const char *topic, const uint8_t *message, uint32_t length) = 0;

private:
    friend class ConnectionManager;
    MessageHandler *next;
};

#endif
