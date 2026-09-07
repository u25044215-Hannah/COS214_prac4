#ifndef ISSUEDECORATOR_H
#define ISSUEDECORATOR_H

#include "Component.h"

class IssueDecorator : public Component {
protected:
    Component* wrappedComponent;

public:
    explicit IssueDecorator(Component* component);

    virtual ~IssueDecorator() = default;

    std::string getState() const override;
    void setState(const std::string& newState) override;

    std::string getName() const override;
    std::string getID() const override;
    std::string getDescription() const override;

    bool hasPriority(const std::string& level) const override;
};

#endif