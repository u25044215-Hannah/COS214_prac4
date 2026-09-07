#ifndef INPROGRESSSTATE_H
#define INPROGRESSSTATE_H

#include "State.h"

class InProgressState : public State {
public:
    InProgressState() = default;
    ~InProgressState() override = default;

    std::string getName() const override;

    std::unique_ptr<State>
    transitionTo(const std::string &newState) const override;
};

#endif