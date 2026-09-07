#ifndef PRIORITYDECORATOR_H
#define PRIORITYDECORATOR_H

#include "IssueDecorator.h"
#include <string>

class PriorityDecorator : public IssueDecorator {
private:
    std::string priorityLevel;

public:
    PriorityDecorator(Component* component, const std::string& level);

    ~PriorityDecorator() override = default;

    bool hasPriority(const std::string& level) const override;

    std::string getDescription() const override;
};

#endif