/**************************************************************************************/
/* Power management                                                                   */
/**************************************************************************************/

#include <Arduino.h>

bool power = false;

#ifdef MEGA
const int voltagePin = A0;
const int controlPin = 10;
#else
const int voltagePin = A0;
const int controlPin = D8;
#endif

#ifdef MEGA
const int LED_ON = HIGH;
const int LED_OFF = LOW;
#else
const int LED_ON = LOW;
const int LED_OFF = HIGH;
#endif

void initializePower() {
  pinMode(controlPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Todo - read power state from EEPROM and revert to that state
}

bool checkPower() {
  int minVoltage = 50;
  int voltage = analogRead(voltagePin);
  // If we're supposed to be powered, and we are, return true
  if (power && voltage > minVoltage) return true;
  // If we're not powered, return false
  if (voltage <= minVoltage) {
    power = false;
    digitalWrite(LED_BUILTIN, LED_OFF);
    return false;
  }
  // Else, we're in the process of powering on
  int timeout = 50; // We'll wait up to 5 seconds for the power voltage to rise
  int powerChecks = 0;
  while (timeout > 0) {
    if (voltage > minVoltage) {
      powerChecks++;
      if (powerChecks > 5) { // We want to see five consecutive high checks
        digitalWrite(LED_BUILTIN, LED_ON);
        power = true;
        return true;
      }
    } else {
      powerChecks = 0;
    }
    delay(100);
    voltage = analogRead(voltagePin);
    timeout--;
  }
  power = false;
  digitalWrite(LED_BUILTIN, LED_OFF);
  return false;
}

void powerOn() {
  digitalWrite(controlPin, HIGH);
  int voltage = analogRead(voltagePin);
  int timeout = 50; // We'll wait up to 5 seconds for the power voltage to rise
  int powerChecks = 0;
  while (timeout > 0) {
    if (voltage > 400) {
      powerChecks++;
      if (powerChecks > 5) { // We want to see five consecutive high checks
        power = true;
        return;
      }
    } else {
      powerChecks = 0;
    }
    delay(100);
    voltage = analogRead(voltagePin);
    timeout--;
  }
}

void powerOff() {
  digitalWrite(controlPin, LOW);
  int voltage = analogRead(voltagePin);
  int timeout = 100; // We'll wait up to 10 seconds for the power voltage to drop
  while (timeout > 0) {
    if (voltage < 40) {
      power = false;
      return;
    }
    delay(100);
    voltage = analogRead(voltagePin);
    timeout--;
  }
}
