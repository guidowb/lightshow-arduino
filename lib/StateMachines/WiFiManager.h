#ifndef _WIFI_MANAGER_H_
#define _WIFI_MANAGER_H_

#include "StateMachine.h"
#include "RateLimiter.h"

class WiFiManager : private StateMachine {
public:
    WiFiManager();

    void setup();
    void loop();

    bool isConnected();

private:
    enum State { WIFI_CONNECTING, WIFI_CONNECTED, WIFI_DISCONNECTED };
    RateLimiter connectionCheck;
    RateLimiter connectionAttempt;

    void enterConnecting();
    void whileConnecting();
    void enterConnected();
    void whileConnected();
    void enterDisconnected();
    void whileDisconnected();
};

#endif
