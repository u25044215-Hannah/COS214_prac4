#include "DepthFirstIterator.h"

DepthFirstIterator::DepthFirstIterator(Component* root)
    : currentPosition(0) {
    buildTraversal(root);
}

void DepthFirstIterator::buildTraversal(Component* component) {
    if (component == nullptr) {
        return;
    }

    if (!visited.insert(component).second) {
        return;
    }

    // First visit the current component
    traversal.push_back(component);

    // Then visit all of its children recursively
    for (int i = 0; i < component->getChildCount(); i++) {
        buildTraversal(component->getChild(i));
    }
}

bool DepthFirstIterator::hasNext() const {
    return currentPosition < traversal.size();
}

Component* DepthFirstIterator::next() {
    if (!hasNext()) {
        return nullptr;
    }

    return traversal[currentPosition++];
}

void DepthFirstIterator::reset() {
    currentPosition = 0;
}