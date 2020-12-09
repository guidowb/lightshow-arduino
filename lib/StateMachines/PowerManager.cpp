#include "PowerManager.h"

#include <Arduino.h>

#ifdef MEGA
const int voltagePin = A0;
const int controlPin = 10;
#else
const int voltagePin = A0;
const int controlPin = D7;
const int offVoltage = 40;
const int onVoltage = 200;
#endif

#ifdef MEGA
const int LED_ON = HIGH;
const int LED_OFF = LOW;
#else
const int LED_ON = LOW;
const int LED_OFF = HIGH;
#endif

PowerManager::PowerManager() : powerCheck(100) {
  setState(POWERED_OFF);
  lastNeeded = millis() - 30000;
}

void PowerManager::setup() {
  pinMode(controlPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(controlPin, LOW);
  digitalWrite(LED_BUILTIN, LED_OFF);
  setState(POWERING_OFF);
}

void PowerManager::loop() {
  switch (getState()) {
  case POWERED_OFF:  checkNeeded();   break;
  case POWERING_OFF: checkPowerOff(); break;
  case POWERING_ON:  checkPowerOn();  break;
  case POWERED_ON:   checkPowered();  break;
  }
}

void PowerManager::checkPowerOff() {
  if (!powerCheck.due()) return;
  int voltage = analogRead(voltagePin);
  if (voltage < offVoltage) {
    Serial.println("Powered off");
    setState(POWERED_OFF);
    digitalWrite(LED_BUILTIN, LED_OFF);
  }
  else if (timeInState(1000) && voltage > onVoltage) {
    Serial.println("Power is not managed");
    setState(POWER_UNMANAGED);
    digitalWrite(LED_BUILTIN, LED_ON);
  }
}

void PowerManager::checkPowerOn() {
  if (!powerCheck.due()) return;
  int voltage = analogRead(voltagePin);
  if (voltage > onVoltage) {
    if (timeInState(500)) {
      Serial.println("Powered on");
      setState(POWERED_ON);
      digitalWrite(LED_BUILTIN, LED_ON);
    }
  }
  else {
    setState(POWERING_ON);
  }
}

void PowerManager::checkPowered() {
  if (millis() - lastNeeded > 30000) {
    powerOff();
    return;
  }
  int voltage = analogRead(voltagePin);
  if (voltage <= onVoltage) {
    Serial.println("Power lost");
    setState(POWERING_ON);
    digitalWrite(LED_BUILTIN, LED_OFF);
    powerCheck.reset();
  }
}

void PowerManager::checkNeeded() {
  if (millis() - lastNeeded < 5000) {
    powerOn();
  } 
}

void PowerManager::powerOn() {
  Serial.println("Power on");
  setState(POWERING_ON);
  digitalWrite(controlPin, HIGH);
  powerCheck.reset();
}

void PowerManager::powerOff() {
  Serial.println("Power off");
  setState(POWERING_OFF);
  digitalWrite(controlPin, LOW);
  powerCheck.reset();
}

bool PowerManager::isPowered() {
  return getState() == POWERED_ON || getState() == POWER_UNMANAGED;
}

void PowerManager::isNeeded() {
  this->lastNeeded = millis();
}
