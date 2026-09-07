#ifndef CLOSEDSTATE_H
#define CLOSEDSTATE_H

#include "State.h"

class ClosedState : public State {
public:
    ClosedState() = default;
    ~ClosedState() override = default;

    std::string getName() const override;

    std::unique_ptr<State>
    transitionTo(const std::string &newState) const override;
};

#endif