#include "CommandHandler.h"

CommandHandler::CommandHandler(LEDStringManager *ledManager) {
    this->ledManager = ledManager;
}

bool CommandHandler::handleMessage(const char *message) {
    return false;
}