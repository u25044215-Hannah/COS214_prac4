#include "UnresolvedIssueIterator.h"

UnresolvedIssueIterator::UnresolvedIssueIterator(Component* root)
    : currentPosition(0) {
    findUnresolved(root);
}

void UnresolvedIssueIterator::findUnresolved(Component* component) {
    if (component == nullptr) {
        return;
    }

    if (!visited.insert(component).second) {
        return;
    }

    // Leaf components represent individual work items.
    if (component->getChildCount() == 0) {
        if (!component->isClosed()) {
            unresolvedItems.push_back(component);
        }

        return;
    }

    // Search recursively through nested groups.
    for (int i = 0; i < component->getChildCount(); i++) {
        findUnresolved(component->getChild(i));
    }
}

bool UnresolvedIssueIterator::hasNext() const {
    return currentPosition < unresolvedItems.size();
}

Component* UnresolvedIssueIterator::next() {
    if (!hasNext()) {
        return nullptr;
    }

    return unresolvedItems[currentPosition++];
}

void UnresolvedIssueIterator::reset() {
    currentPosition = 0;
}