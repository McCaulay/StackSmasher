#pragma once

#include <sys/wait.h>

class ProcessState
{
public:
	ProcessState(int state);
    int getState();

    bool isPaused();
    bool hasExited();
    bool wasContinued();
    bool hasSignaled();

    int getPauseSignal();
    int getTerminateSignal();
private:
    int state;
};