#ifndef PROJECTITERATOR_H
#define PROJECTITERATOR_H

class Component;

/**
 * @brief Abstract iterator interface for traversing TaskForge components.
 */
class ProjectIterator {
public:
    virtual ~ProjectIterator() = default;

    // Returns true if there is another component to visit
    virtual bool hasNext() const = 0;

    // Returns the next component in the traversal
    virtual Component* next() = 0;

    // Moves the iterator back to the beginning
    virtual void reset() = 0;
};

#endif