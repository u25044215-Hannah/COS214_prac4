
#include "Component.h"
#include "State.h"
#include <iostream>
#include <memory>

using namespace std;

void Component::add(Component *component) {
  cout << "Unable to add component\n";
}

void Component::remove(Component *component) {
  cout << "Unable to remove component\n";
}

Component *Component::getChild(int index) const { return nullptr; }

int Component::getChildCount() const { return 0; }

string Component::getName() const { return name; }

string Component::getID() const { return id; }

string Component::getDescription() const { return description; }

unique_ptr<ProjectIterator> Component::createIterator() const {
  return nullptr;
}

Component::Component(const string &name, const string &id, const string &desc)
    : name(name), id(id), description(desc) {}

bool Component::isOpen() const { return (state->getName() == "Open"); }

bool Component::isInProgress() const {
  return (state->getName() == "InProgress");
}

bool Component::isClosed() const { return (state->getName() == "Closed"); }

bool Component::isAssigned() const { return (state->getName() == "Assigned"); }

bool Component::hasPriority(const string &level) const { return false; }

State *Component::getStateObject() const { return state.get(); }

void Component::setStateObject(unique_ptr<State> newState) {
  state = std::move(newState);
}
