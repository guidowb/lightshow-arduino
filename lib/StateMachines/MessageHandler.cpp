#include "MessageHandler.h"

bool MessageHandler::startsWith(const char *message, const char *word) {
    for (int i = 0;; i++) {
        if (word[i] == '\0') return true;
        if (message[i] == '\0') return false;
        if (message[i] != word[i]) return false;
    }
}

