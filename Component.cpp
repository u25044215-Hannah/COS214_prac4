
#include "Component.h"
#include "ProjectIterator.h"
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
    : name(name), id(id), description(desc), parent(nullptr) {}
    
Component::~Component() = default;

bool Component::isOpen() const { return (getState() == "Open" || getState() == "Assigned"); }

bool Component::isInProgress() const { return (getState() == "InProgress"); }

bool Component::isClosed() const { return (getState() == "Closed"); }

bool Component::isAssigned() const { return (getState() == "Assigned"); }

bool Component::hasPriority(const string &level) const { return false; }

State *Component::getStateObject() const { return state.get(); }

void Component::setStateObject(unique_ptr<State> newState) {
  state = std::move(newState);
}

Component *Component::getParent() const { return parent; }

void Component::setParent(Component *newParent) { parent = newParent; }

void Component::replaceChild(Component* oldChild, Component* newChild) {
    cout << "Unable to replace child on issues";
}
