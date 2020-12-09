#include "CommandHandler.h"

static bool startsWith(const char *message, const char *word) {
    for (int i = 0;; i++) {
        if (word[i] == '\0') return true;
        if (message[i] == '\0') return false;
        if (message[i] != word[i]) return false;
    }
}

CommandHandler::CommandHandler(LEDStringManager *ledManager) {
    this->ledManager = ledManager;
}

bool CommandHandler::handleMessage(const char *message) {
    if (startsWith(message, "pattern\n")) {
        ledManager->setPattern(message + 8);
        return true;
    }
    return false;
}