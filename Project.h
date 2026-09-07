#ifndef PROJECT_H
#define PROJECT_H
#include "Component.h"

class Project : public Component {
    private:
    std::string deadline;
    int estimatedHours;

    public:
  Project(const std::string &name, const std::string &id, const std::string &desc,
       std::string deadline, int estimatedHours);
  virtual ~Project() = default;
  virtual std::string getDescription() const override;
};

#endif