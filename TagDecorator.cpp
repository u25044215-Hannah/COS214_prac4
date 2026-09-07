#include "TagDecorator.h"

TagDecorator::TagDecorator(
    Component* component,
    const std::string& tagName)
    : IssueDecorator(component),
      tag(tagName) {
}

std::string TagDecorator::getDescription() const {
    return (wrappedComponent ? wrappedComponent->getDescription() : description)
        + " [Tag: " + tag + "]";
}