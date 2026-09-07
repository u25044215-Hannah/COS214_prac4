#include "IssueDecorator.h"

IssueDecorator::IssueDecorator(Component* component)
    : Component(
          component ? component->getName() : "",
          component ? component->getID() : "",
          component ? component->getDescription() : ""),
      wrappedComponent(component) {
}

std::string IssueDecorator::getState() const {
    return wrappedComponent ? wrappedComponent->getState() : "Unknown";
}

void IssueDecorator::setState(const std::string& newState) {
    if (wrappedComponent) {
        wrappedComponent->setState(newState);
    }
}

std::string IssueDecorator::getName() const {
    return wrappedComponent ? wrappedComponent->getName() : name;
}

std::string IssueDecorator::getID() const {
    return wrappedComponent ? wrappedComponent->getID() : id;
}

std::string IssueDecorator::getDescription() const {
    return wrappedComponent ? wrappedComponent->getDescription() : description;
}

bool IssueDecorator::hasPriority(const std::string& level) const {
    return wrappedComponent && wrappedComponent->hasPriority(level);
}