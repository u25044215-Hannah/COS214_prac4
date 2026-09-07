#ifndef UNRESOLVEDISSUEITERATOR_H
#define UNRESOLVEDISSUEITERATOR_H

#include "ProjectIterator.h"
#include "Component.h"

#include <cstddef>
#include <unordered_set>
#include <vector>

/**
 * @brief Traverses only unresolved leaf components.
 *
 * This iterator uses a snapshot traversal policy.
 */
class UnresolvedIssueIterator : public ProjectIterator {

private:
    std::vector<Component*> unresolvedItems;
    std::unordered_set<Component*> visited;
    std::size_t currentPosition;

    void findUnresolved(Component* component);

public:
    explicit UnresolvedIssueIterator(Component* root);
    virtual ~UnresolvedIssueIterator() = default;

    bool hasNext() const override;
    Component* next() override;
    void reset() override;
};

#endif