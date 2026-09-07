#ifndef ASSIGNEDSTATE_H
#define ASSIGNEDSTATE_H

#include "State.h"

class AssignedState : public State {
public:
    AssignedState() = default;
    ~AssignedState() override = default;

    std::string getName() const override;

    std::unique_ptr<State>
    transitionTo(const std::string &newState) const override;
};

#endif