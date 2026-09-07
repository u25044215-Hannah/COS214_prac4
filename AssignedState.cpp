#include "AssignedState.h"
#include "InProgressState.h"

std::string AssignedState::getName() const {
    return "Assigned";
}

std::unique_ptr<State>
AssignedState::transitionTo(const std::string &newState) const {

    if (newState == "InProgress") {
        return std::unique_ptr<State>(
            new InProgressState()
        );
    }

    return std::unique_ptr<State>();
}