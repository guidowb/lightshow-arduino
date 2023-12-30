#include "UpdateManager.h"
#include "LogManager.h"
#include "Variables.h"

#include "ArduinoOTA.h"

static Logger logger("update");

UpdateManager::UpdateManager(WiFiManager *wifiManager) : wifiManager(wifiManager) {
    setState(DISABLED);
}

void UpdateManager::setup() {}

void UpdateManager::enable() {
    ArduinoOTA.setHostname(ota_hostname);
    ArduinoOTA.setPassword(ota_password);

    ArduinoOTA.onStart([this]() {
        this->setState(UPDATING);
    });

    ArduinoOTA.onEnd([this]() {
        this->setState(DISABLED);
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    });

    ArduinoOTA.onError([this](ota_error_t error) {
    // ---- It doesn't appear that we can safely log from inside any of these -----
        // if (error == OTA_AUTH_ERROR) logger.error("Update auth failed");
        // else if (error == OTA_BEGIN_ERROR) logger.error("Update begin failed");
        // else if (error == OTA_CONNECT_ERROR) logger.error("Update connect failed");
        // else if (error == OTA_RECEIVE_ERROR) logger.error("Update receive failed");
        // else if (error == OTA_END_ERROR) logger.error("Update end failed");
        // else logger.error("Update failed with error code %u", error);
        this->setState(DISABLED);
    });
    ArduinoOTA.begin();

    setState(ENABLED);
}

void UpdateManager::loop() {
    do {
        if (getState() == DISABLED && wifiManager->isConnected()) enable();
        if (getState() != DISABLED) ArduinoOTA.handle();
    } while (getState() == UPDATING);
}
