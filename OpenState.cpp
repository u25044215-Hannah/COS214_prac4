#include "OpenState.h"
#include "AssignedState.h"

std::string OpenState::getName() const {
    return "Open";
}

std::unique_ptr<State>
OpenState::transitionTo(const std::string &newState) const {

    if (newState == "Assigned") {
        return std::unique_ptr<State>(
            new AssignedState()
        );
    }

    return std::unique_ptr<State>();
}