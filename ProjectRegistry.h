#ifndef PROJECTREGISTRY_H
#define PROJECTREGISTRY_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "IssueDecorator.h"

class IssueDecorator;
class Component;
class TemplateCreator;

class ProjectRegistry {
private:
  std::unordered_map<std::string, std::unique_ptr<Component>> lookup;
  std::unordered_map<std::string, std::unique_ptr<Component>> decoratedComponents;

public:
  ProjectRegistry() = default;
  ~ProjectRegistry() = default;

  // Lookup
  Component *getComponent(const std::string &id) const;
  bool exists(const std::string &id) const;

  // Factory Methods
  void createProject(const std::string &name, const std::string &id,
                     const std::string &deadline, int estimatedHours,
                     const std::string &desc = "");
  void createRepo(const std::string &name, const std::string &id,
                  const std::string &desc, std::string owner, bool privateRepo,
                  std::string language);
  void createIssue(const std::string &name, const std::string &id, 
                   const std::string &desc = "");

  // Structure Operations
  void addChild(const std::string &parentId, const std::string &childId);
  void removeChild(const std::string &parentId, const std::string &childId);
  void moveChild(const std::string &childId, const std::string &newParentId);

  // State Operations
  void setState(const std::string &id, const std::string &newState);
  std::string getState(const std::string &id) const;

  // Decoration
  void decorateIssue(const std::string &issueId,
                     std::unique_ptr<IssueDecorator> decorator);

  // Traversal Helpers
  std::vector<std::string> getChildIds(const std::string &parentId) const;

  // Debug / Utility
  void printTree() const;
  void printNode(const Component* node, int depth) const;
};

#endif