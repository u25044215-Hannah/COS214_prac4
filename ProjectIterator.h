#ifndef PROJECTITERATOR_H
#define PROJECTITERATOR_H

/**
 * @brief Abstract iterator interface used to traverse TaskForge components.
 *
 * Concrete iterators implement different ways of traversing
 * the project hierarchy.
 */
template <typename T>
class ProjectIterator {
public:
    virtual ~ProjectIterator() = default;

    // Returns true if there is another component to visit
    virtual bool hasNext() const = 0;

    // Returns the next component in the traversal
    virtual T* next() = 0;

    // Moves the iterator back to the beginning
    virtual void reset() = 0;
};

#endif