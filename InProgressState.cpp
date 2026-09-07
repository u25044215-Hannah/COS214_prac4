#include "InProgressState.h"
#include "ReviewState.h"

std::string InProgressState::getName() const {
    return "InProgress";
}

std::unique_ptr<State>
InProgressState::transitionTo(const std::string &newState) const {

    if (newState == "Review") {
        return std::unique_ptr<State>(
            new ReviewState()
        );
    }

    return std::unique_ptr<State>();
}