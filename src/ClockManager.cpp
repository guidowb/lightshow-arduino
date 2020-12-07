#include "ClockManager.h"

#ifndef MEGA

#include <NTPClient.h>
#include <WiFiUdp.h>
#include "RateLimiter.h"

static WiFiUDP ntpUDP;
static NTPClient timeClient(ntpUDP);
static RateLimiter updateLimiter(30000);

void ClockManager::setup() {
    timeClient.begin();
    timeClient.setUpdateInterval(3600);
}

void ClockManager::loop() {
    if (updateLimiter.due()) {
        timeClient.update();
#ifdef SHOWCLOCK
        long current = timeClient.getEpochTime();
        if (current - last >= 1800) {
            char *time = "Current time is 00:00:00";
            last = current;
            int hours = (timeClient.getHours() + TIMEZONE_OFFSET + 24) % 24;
            int minutes = timeClient.getMinutes();
            int seconds = timeClient.getSeconds();
            time[16] = (hours / 10) + '0';
            time[17] = (hours % 10) + '0';
            time[19] = (minutes / 10) + '0';
            time[20] = (minutes % 10) + '0';
            time[22] = (seconds / 10) + '0';
            time[23] = (seconds % 10) + '0';
            Serial.println(time);
        }
#endif
    }
}

#endif