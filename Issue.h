#ifndef ISSUE_H
#define ISSUE_H

#include "Component.h"

class Issue : Component {

public:
  Issue(const std::string &name, const std::string &id,
        const std::string &desc = "");
  virtual ~Issue() = default;

  std::string getState() const override;
  void setState(const std::string &newState) override;
};

#endif