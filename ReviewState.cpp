#include "ReviewState.h"
#include "ClosedState.h"
#include "InProgressState.h"

std::string ReviewState::getName() const {
    return "Review";
}

std::unique_ptr<State>
ReviewState::transitionTo(const std::string &newState) const {

    // Reviewer approves the work.
    if (newState == "Closed") {
        return std::unique_ptr<State>(
            new ClosedState()
        );
    }

    // Reviewer rejects it and sends it back for more work.
    if (newState == "InProgress") {
        return std::unique_ptr<State>(
            new InProgressState()
        );
    }

    return std::unique_ptr<State>();
}