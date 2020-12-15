#include "ClockManager.h"

#include <NTPClient.h>
#include <WiFiUdp.h>
#include "RateLimiter.h"

#ifndef TIMEZONE_OFFSET
#define TIMEZONE_OFFSET 0
#endif

static WiFiUDP ntpUDP;
static NTPClient timeClient(ntpUDP);
static RateLimiter updateLimiter(30000);

void ClockManager::setup() {
    updated = false;
    timeClient.begin();
    timeClient.setTimeOffset(TIMEZONE_OFFSET * 3600);
    timeClient.setUpdateInterval(3600);
}

void ClockManager::loop() {
    if (updateLimiter.due()) {
        if (timeClient.update()) updated = true;
    }
}

uint32_t ClockManager::getEpochTime() {
    if (updated) return timeClient.getEpochTime();
    else return 0;
}
