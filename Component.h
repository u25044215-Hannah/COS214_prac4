#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <string>
#include <vector>

class State;
class ProjectIterator;

/**
 * @brief This class acts at the Component in the composite hierarchy.
 *
 */
class Component {

private:
  std::unique_ptr<State> state;
  Component *parent;

protected:
  std::string name;
  std::string id;
  std::string description;

public:
  // Composite

  /**
   * @brief Add a component to the current one as child
   *
   * @param component
   */
  virtual void add(Component *component);
  /**
   * @brief Remove the component from the current one
   *
   * @param component
   */
  virtual void remove(Component *component);
  /**
   * @brief Get the Child object
   *
   * @param index
   * @return Component*
   */
  virtual Component *getChild(int index) const;
  /**
   * @brief Get the number of children
   *
   * @return int
   */
  virtual int getChildCount() const;

  // State

  /**
   * @brief Get the name of the current state
   *
   * @return std::string
   */
  virtual std::string getState() const = 0;
  /**
   * @brief Set the State
   *
   * @param newState
   */
  virtual void setState(const std::string &newState) = 0;

  // Core identity

  /**
   * @brief Get the name
   *
   * @return std::string
   */
  virtual std::string getName() const;
  /**
   * @brief Get the ID
   *
   * @return std::string
   */
  virtual std::string getID() const;
  /**
   * @brief Get the description
   *
   * @return std::string
   */
  virtual std::string getDescription() const;

  /**
   * @brief Create an Iterator
   *
   * @return std::unique_ptr<ProjectIterator<Component>>
   */
  virtual std::unique_ptr<ProjectIterator> createIterator() const;

  /**
   * @brief Construct a new Component object
   *
   * @param name
   * @param id
   * @param desc
   */
  Component(const std::string &name, const std::string &id,
            const std::string &desc = "");
  virtual ~Component() ;

  /**
   * @brief Checks if the object is in open state
   *
   * @return true
   * @return false
   */
  bool isOpen() const;
  /**
   * @brief Checks if object is in progress
   *
   * @return true
   * @return false
   */
  bool isInProgress() const;
  /**
   * @brief Checks if the object is closed
   *
   * @return true
   * @return false
   */
  bool isClosed() const;

  /**
   * @brief Checks if the object has been assigned
   *
   * @return true
   * @return false
   */
  bool isAssigned() const;

  /**
   * @brief Checks for the presence of the priority decorator
   *
   * @param level
   * @return true
   * @return false
   */
  virtual bool hasPriority(const std::string &level) const;

  /**
   * @brief Get the Parent object
   *
   * @return Component*
   */
  virtual Component *getParent() const;

  /**
   * @brief Set the Parent object
   *
   * @param parent
   */
  virtual void setParent(Component *parent);

  /**
   * @brief Replaces the desired child with another. For the decorator pattern
   *
   * @param oldChild
   * @param newChild
   */
  virtual void replaceChild(Component *oldChild, Component *newChild);

protected:
  // Protected accessor for derived classes to manipulate the state pointer
  State *getStateObject() const;
  void setStateObject(std::unique_ptr<State> newState);
};

#endif