#ifndef TAGDECORATOR_H
#define TAGDECORATOR_H

#include "IssueDecorator.h"
#include <string>

class TagDecorator : public IssueDecorator {
private:
    std::string tag;

public:
    TagDecorator(Component* component, const std::string& tagName);

    ~TagDecorator() override = default;

    std::string getDescription() const override;
};

#endif