#include "StateMachine.h"

#include <Arduino.h>

void StateMachine::setState(int state) {
    this->state = state;
    this->stateEntered = millis();
}

int StateMachine::getState() {
    return this->state;
}

bool StateMachine::timeInState(long threshold) {
    return (millis() - this->stateEntered) > threshold;
}
