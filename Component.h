#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <string>
#include <vector>

class State;
template <typename T> class ProjectIterator;

class Component {
public:
  // Composite
  virtual void add(Component *component);
  virtual void remove(Component *component);
  virtual Component *getChild(int index) const;
  virtual int getChildCount() const;

  // State
  virtual std::string getState() const = 0;
  virtual void setState(const std::string& newState) = 0;

  // Core identity
  virtual std::string getName() const;
  virtual std::string getID() const;
  virtual std::string getDescription() const;

  virtual std::unique_ptr<ProjectIterator<Component>> createIterator() const;

  Component(const std::string &name, const std::string &id,
            const std::string &desc = "");
  virtual ~Component() = default;


  bool isOpen() const;
  bool isInProgress() const;
  bool isClosed() const;
  virtual bool hasPriority(const std::string& level) const;

protected:
  // Protected accessor for derived classes to manipulate the state pointer
  State *getStateObject() const;
  void setStateObject(std::unique_ptr<State> newState);

private:
  std::unique_ptr<State> state;
  std::string name;
  std::string id;
  std::string description;
  Component *parent;
};

#endif