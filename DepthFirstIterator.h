#ifndef DEPTHFIRSTITERATOR_H
#define DEPTHFIRSTITERATOR_H

#include "ProjectIterator.h"
#include "Component.h"

#include <cstddef>
#include <vector>

/**
 * @brief Traverses the entire project hierarchy in depth-first order.
 *
 * The iterator uses a snapshot of the traversal order when it is created.
 * This means changes to the hierarchy do not change an iterator that is
 * already in progress.
 */
class DepthFirstIterator : public ProjectIterator<Component> {

private:
    std::vector<Component*> traversal;
    std::size_t currentPosition;

    // Recursively builds the depth-first traversal
    void buildTraversal(Component* component);

public:
    explicit DepthFirstIterator(Component* root);
    virtual ~DepthFirstIterator() = default;

    bool hasNext() const override;
    Component* next() override;
    void reset() override;
};

#endif