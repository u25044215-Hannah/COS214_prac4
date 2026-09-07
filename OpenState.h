#ifndef OPENSTATE_H
#define OPENSTATE_H

#include "State.h"

class OpenState : public State {
public:
    OpenState() = default;
    ~OpenState() override = default;

    std::string getName() const override;

    std::unique_ptr<State>
    transitionTo(const std::string &newState) const override;
};

#endif