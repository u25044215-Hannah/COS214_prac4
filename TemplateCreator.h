#ifndef TEMPLATECREATOR_H
#define TEMPLATECREATOR_H

#include "ProjectRegistry.h"
class TemplateCreator {
public:
  void build(ProjectRegistry *registry);
  TemplateCreator() = default;
  virtual ~TemplateCreator() = default;
};

#endif