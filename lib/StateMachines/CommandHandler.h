#ifndef _COMMAND_HANDLER_H_
#define _COMMAND_HANDLER_H_

#include <ConnectionManager.h>
#include <LEDStringManager.h>

class CommandHandler : public MessageHandler {
public:
    CommandHandler(LEDStringManager *ledManager);
    virtual bool handleMessage(const char *message);

private:
    LEDStringManager *ledManager;
};

#endif
