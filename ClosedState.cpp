#include "ClosedState.h"
#include "OpenState.h"

std::string ClosedState::getName() const {
    return "Closed";
}

std::unique_ptr<State>
ClosedState::transitionTo(const std::string &newState) const {

    if (newState == "Open") {
        return std::unique_ptr<State>(
            new OpenState()
        );
    }

    return std::unique_ptr<State>();
}