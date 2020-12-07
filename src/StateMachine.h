#ifndef _STATEMACHINE_H_
#define _STATEMACHINE_H_

class StateMachine {
public:
    void setState(int state);
    int  getState();
    bool timeInState(long threshold);

private:
    int  state;
    long stateEntered;
};

#endif