#ifndef STATE_H
#define STATE_H

#include <memory>
#include <string>

/**
 * @brief Abstract State participant for an Issue.
 *
 * Each concrete State controls which state transitions are
 * valid from that particular state.
 */
class State {
public:
    virtual ~State() = default;

    /**
     * @brief Get the human-readable name of this state.
     *
     * @return Name of the state.
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Request a transition to another state.
     *
     * If the transition is valid, a new State object is returned.
     * If the transition is invalid, nullptr is returned.
     *
     * @param newState Name of the requested target state.
     * @return std::unique_ptr<State>
     */
    virtual std::unique_ptr<State>
    transitionTo(const std::string &newState) const = 0;
};

#endif