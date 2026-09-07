#include "IssueDecorator.h"

IssueDecorator::IssueDecorator(Component* component)
    : Component(
          component->getName(),
          component->getID(),
          component->getDescription()),
      wrappedComponent(component) {
}

std::string IssueDecorator::getState() const {
    return wrappedComponent->getState();
}

void IssueDecorator::setState(const std::string& newState) {
    wrappedComponent->setState(newState);
}

std::string IssueDecorator::getName() const {
    return wrappedComponent->getName();
}

std::string IssueDecorator::getID() const {
    return wrappedComponent->getID();
}

std::string IssueDecorator::getDescription() const {
    return wrappedComponent->getDescription();
}

bool IssueDecorator::hasPriority(const std::string& level) const {
    return wrappedComponent->hasPriority(level);
}