#include "PowerManager.h"

#include <Arduino.h>

#ifdef MEGA
const int voltagePin = A0;
const int controlPin = 10;
#else
const int voltagePin = A0;
const int controlPin = D7;
const int offVoltage = 100;
const int onVoltage  = 200;
#endif

#ifdef MEGA
const int LED_ON = HIGH;
const int LED_OFF = LOW;
#else
const int LED_ON = LOW;
const int LED_OFF = HIGH;
#endif

PowerManager::PowerManager() : powerCheck(100), autoShutoff(30000) {
  setState(POWERED_OFF);
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
  case POWERED_OFF:     whilePoweredOff();  break;
  case POWERING_OFF:    whilePoweringOff(); break;
  case POWERING_ON:     whilePoweringOn();  break;
  case POWERED_ON:      whilePoweredOn();   break;
  case POWER_LOST:      whilePowerLost();   break;
  case POWER_UNMANAGED: whileUnmanaged();   break;
  }
}

void PowerManager::enterPoweringOff() {
  Serial.println("Powering off");
  setState(POWERING_OFF);
  digitalWrite(controlPin, LOW);
  powerCheck.reset();
}

void PowerManager::whilePoweringOff() {
  if (!powerCheck.due()) return;
  int voltage = analogRead(voltagePin);
  if (voltage < offVoltage) {
    enterPoweredOff();
    return;
  }
  else if (timeInState(5000) && voltage > onVoltage) {
    enterUnmanaged();
    return;
  }
}

void PowerManager::enterPoweringOn() {
  Serial.println("Powering on");
  setState(POWERING_ON);
  digitalWrite(controlPin, HIGH);
  autoShutoff.reset();
  powerCheck.reset();
}

void PowerManager::whilePoweringOn() {
  if (!powerCheck.due()) return;
  int voltage = analogRead(voltagePin);
  if (voltage > onVoltage) {
    if (timeInState(500)) {
      enterPoweredOn();
      return;
    }
  }
  else {
    setState(POWERING_ON);
    if (autoShutoff.due()) {
      enterPoweringOff();
      return;
    }
  }
}

void PowerManager::enterPoweredOn() {
  Serial.println("Powered on");
  setState(POWERED_ON);
  digitalWrite(LED_BUILTIN, LED_ON);
}

void PowerManager::whilePoweredOn() {
  if (!powerCheck.due()) return;
  if (autoShutoff.due()) {
    enterPoweringOff();
    return;
  }
  int voltage = analogRead(voltagePin);
  if (voltage <= onVoltage) {
    enterPowerLost();
    return;
  }
}

void PowerManager::enterPoweredOff() {
  Serial.println("Powered off");
  setState(POWERED_OFF);
  digitalWrite(LED_BUILTIN, LED_OFF);
}

void PowerManager::whilePoweredOff() {
  if (millis() - lastNeeded < 5000) {
    enterPoweringOn();
  } 
}

void PowerManager::enterPowerLost() {
  Serial.println("Power lost");
  setState(POWER_LOST);
  digitalWrite(LED_BUILTIN, LED_OFF);
  powerCheck.reset();
}

void PowerManager::whilePowerLost() {
  if (!powerCheck.due()) return;
  int voltage = analogRead(voltagePin);
  if (voltage > onVoltage) {
    enterPoweringOn();
    return;
  }
}

void PowerManager::enterUnmanaged() {
  Serial.println("Power is not managed");
  setState(POWER_UNMANAGED);
  digitalWrite(LED_BUILTIN, LED_ON);
}

void PowerManager::whileUnmanaged() {
  if (!powerCheck.due()) return;
  int voltage = analogRead(voltagePin);
  if (voltage <= onVoltage) {
    enterPowerLost();
    return;
  }
}

void PowerManager::powerOn() {
  enterPoweringOn();
}

void PowerManager::powerOff() {
  enterPoweringOff();
}

void PowerManager::powerNeeded() {
  autoShutoff.reset();
  if (!isPowered()) enterPoweringOn();
}

bool PowerManager::isPowered() {
  return getState() == POWERED_ON || getState() == POWER_UNMANAGED;
}
