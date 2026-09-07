#include "PriorityDecorator.h"

PriorityDecorator::PriorityDecorator(
    Component* component,
    const std::string& level)
    : IssueDecorator(component),
      priorityLevel(level) {
}

bool PriorityDecorator::hasPriority(const std::string& level) const {
    // This decorator provides its own priority.
    // If it does not match, continue checking the wrapped component.
    return priorityLevel == level ||
           (wrappedComponent && wrappedComponent->hasPriority(level));
}

std::string PriorityDecorator::getDescription() const {
    return (wrappedComponent ? wrappedComponent->getDescription() : description)
        + " [Priority: " + priorityLevel + "]";
}