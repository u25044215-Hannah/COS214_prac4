
#include "Project.h"
#include "Composite.h"
#include <string>

using namespace std;

Project::Project(const string &name, const string &id, const string &desc,
                 string deadline, int estimatedHours)
    : Composite(name, id, desc), deadline(deadline),
      estimatedHours(estimatedHours) {}

string Project::getDescription() const {
  return description + " [Deadline: " + deadline +
         ", Est: " + to_string(estimatedHours) + "h]";
}
