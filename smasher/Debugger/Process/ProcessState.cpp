#include "Debugger/Process/ProcessState.hpp"

ProcessState::ProcessState(int state)
{
    this->state = state;
}

int ProcessState::getState()
{
    return this->state;
}

bool ProcessState::isPaused()
{
    return WIFSTOPPED(this->state);
}

bool ProcessState::hasExited()
{
    return WIFEXITED(this->state);
}

bool ProcessState::wasContinued()
{
    return WIFCONTINUED(this->state);
}

bool ProcessState::hasSignaled()
{
    return WIFSIGNALED(this->state);
}

int ProcessState::getPauseSignal()
{
    return WSTOPSIG(this->state);
}

int ProcessState::getTerminateSignal()
{
    return WTERMSIG(this->state);
}
