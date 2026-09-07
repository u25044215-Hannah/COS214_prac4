#include "Composite.h"
#include "Component.h"
#include "DepthFirstIterator.h"
#include <algorithm>
#include <memory>

using namespace std;

Composite::Composite(const string &name, const string &id, const string &desc)
    : Component(name, id, desc) {}

string Composite::getState() const {
  if (children.empty()) {
    return "Open";
  }

  // Check for InProgress children
  for (auto child : children) {
    if (!child) {
      continue;
    }
    if (child->getState() == "Review") {
      return "Review";
    }
  }

  // Check for InProgress children
  for (auto child : children) {
    if (!child) {
      continue;
    }
    if (child->getState() == "InProgress") {
      return "InProgress";
    }
  }

  // Check for Assigned children
  for (auto child : children) {
    if (!child) {
      continue;
    }
    if (child->getState() == "Assigned") {
      return "Assigned";
    }
  }

  // Check if all children are Closed
  bool allClosed = true;
  for (auto child : children) {
    if (!child) {
      continue;
    }
    if (child->getState() != "Closed") {
      allClosed = false;
      break;
    }
  }
  if (allClosed) {
    return "Closed";
  }

  return "Open";
}

void Composite::setState(const string &newState) {
  for (auto child : children) {
    if (!child) {
      continue;
    }
    child->setState(newState);
  }
}

void Composite::add(Component *child) {
  if (!child || child == this || child->getParent() != nullptr ||
      find(children.begin(), children.end(), child) != children.end()) {
    return;
  }

  for (Component *ancestor = this; ancestor != nullptr;
       ancestor = ancestor->getParent()) {
    if (ancestor == child) {
      return;
    }
  }

  children.push_back(child);
  child->setParent(this);
}

void Composite::remove(Component *child) {
  if (!child)
    return;
  auto it = find(children.begin(), children.end(), child);
  if (it != children.end()) {
    children.erase(it);
    child->setParent(nullptr);
  }
}

void Composite::replaceChild(Component *oldChild, Component *newChild) {
  if (!oldChild || !newChild || oldChild == newChild ||
      newChild == this || newChild->getParent() != nullptr) {
    return;
  }

  for (Component *ancestor = this; ancestor != nullptr;
       ancestor = ancestor->getParent()) {
    if (ancestor == newChild) {
      return;
    }
  }

  auto it = find(children.begin(), children.end(), oldChild);
  if (it == children.end()) {
    return;
  }

  *it = newChild;
  oldChild->setParent(nullptr);
  newChild->setParent(this);
}

Component *Composite::getChild(int index) const {
  if (index >= static_cast<int>(children.size()) || index < 0) {
    return nullptr;
  }
  return children[index];
}

int Composite::getChildCount() const {
  return static_cast<int>(children.size());
}

unique_ptr<ProjectIterator> Composite::createIterator() const {
  return unique_ptr<ProjectIterator>(
      new DepthFirstIterator(const_cast<Composite *>(this)));
}
