#include "RateLimiter.h"

#include <Arduino.h>

RateLimiter::RateLimiter(long everyMillis) {
    this->everyMillis = everyMillis;
    this->lastMillis = millis();
}

bool RateLimiter::due() {
    long now = millis();
    if (now - lastMillis >= everyMillis) {
        lastMillis = now;
        return true;
    }
    return false;
}

void RateLimiter::reset() {
    lastMillis = millis();
}
