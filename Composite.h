#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "Component.h"
#include <vector>

/**
 * @brief This class is the composite in the hierachy. It is non owning.
 * 
 */
class Composite : Component {
protected:
  std::vector<Component *> children;

public:
  Composite(const std::string &name, const std::string &id,
            const std::string &desc = "");
  virtual ~Composite() = default;

  /**
   * @brief Aggregates the children states
   * 
   * @return std::string 
   */
  std::string getState() const override;
  void setState(const std::string &newState) override;

  void add(Component *child) override;
  void remove(Component *child) override;
  Component *getChild(int index) const override;
  int getChildCount() const override;

  std::unique_ptr<ProjectIterator> createIterator() const override;
};

#endif