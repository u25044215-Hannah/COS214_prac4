#include "Issue.h"

#include "OpenState.h"
#include "State.h"

#include <iostream>
#include <memory>
#include <utility>

Issue::Issue(const std::string &name,
             const std::string &id,
             const std::string &desc)
    : Component(name, id, desc) {

    // Every new GitHub issue begins in the Open state.
    setStateObject(
        std::unique_ptr<State>(new OpenState())
    );
}

std::string Issue::getState() const {

    State *currentState = getStateObject();

    if (currentState == nullptr) {
        return "Unknown";
    }

    return currentState->getName();
}

void Issue::setState(const std::string &newState) {

    State *currentState = getStateObject();

    if (currentState == nullptr) {
        std::cout
            << "[STATE ERROR] Issue "
            << getID()
            << " does not have a current state."
            << std::endl;

        return;
    }

    const std::string oldState =
        currentState->getName();

    /*
     * The Issue does NOT decide whether the transition
     * is valid.
     *
     * It asks its current State object.
     */
    std::unique_ptr<State> nextState =
        currentState->transitionTo(newState);

    if (nextState == nullptr) {

        std::cout
            << "[INVALID TRANSITION] Issue "
            << getID()
            << " cannot move from "
            << oldState
            << " to "
            << newState
            << "."
            << std::endl;

        return;
    }

    // Replace the current state.
    // unique_ptr automatically destroys the old State.
    setStateObject(std::move(nextState));

    std::cout
        << "[STATE CHANGE] Issue "
        << getID()
        << ": "
        << oldState
        << " -> "
        << getState()
        << std::endl;
}


// -----------------------------------------------------
// GitHub lifecycle operations
// -----------------------------------------------------

void Issue::assign() {
    setState("Assigned");
}

void Issue::startWork() {
    setState("InProgress");
}

void Issue::submitForReview() {
    setState("Review");
}

void Issue::approve() {
    setState("Closed");
}

void Issue::reject() {
    setState("InProgress");
}

void Issue::reopen() {
    setState("Open");
}