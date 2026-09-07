#ifndef REVIEWSTATE_H
#define REVIEWSTATE_H

#include "State.h"

class ReviewState : public State {
public:
    ReviewState() = default;
    ~ReviewState() override = default;

    std::string getName() const override;

    std::unique_ptr<State>
    transitionTo(const std::string &newState) const override;
};

#endif