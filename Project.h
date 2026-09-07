#ifndef PROJECT_H
#define PROJECT_H
#include "Composite.h"

/**
 * @brief This is the midlevel composite which groups issues into a larger
 * project
 *
 */
class Project : public Composite {
private:
  std::string deadline;
  int estimatedHours;

public:
  Project(const std::string &name, const std::string &id,
          const std::string &desc, std::string deadline, int estimatedHours);
  virtual ~Project() = default;
  virtual std::string getDescription() const override;
};

#endif