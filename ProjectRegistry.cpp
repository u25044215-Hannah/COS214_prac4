
#include "ProjectRegistry.h"
#include "Component.h"
#include "Issue.h"
#include "Project.h"
#include "Repo.h"
#include <iostream>
#include <memory>

using namespace std;

ProjectRegistry::ProjectRegistry(unique_ptr<TemplateCreator> creator) {
  this->creator = std::move(creator);
}

Component *ProjectRegistry::getComponent(const string &id) const {
  auto it = lookup.find(id);
  if (it == lookup.end()) {
    return nullptr;
  }
  return it->second.get();
}

bool ProjectRegistry::exists(const string &id) const {
  return lookup.find(id) != lookup.end();
}

void ProjectRegistry::createProject(const string &id, const string &name,
                                    const string &deadline, int estimatedHours,
                                    const string &desc) {

  lookup.emplace(id, unique_ptr<Component>(new Project(name, id, desc, deadline,
                                                       estimatedHours)));
}

void ProjectRegistry::createRepo(const string &name, const string &id,
                                 const string &desc, string owner,
                                 bool privateRepo, string language) {
  lookup.emplace(id, unique_ptr<Component>(new Repo(name, id, desc, owner,
                                                    privateRepo, language)));
}

void ProjectRegistry::createIssue(const string &name, const string &id,
                                  const string &desc) {
  lookup.emplace(id, unique_ptr<Component>(new Issue(name, id, desc)));
}

void ProjectRegistry::addChild(const string &parentId, const string &childId) {
  auto parentI = lookup.find(parentId);
  auto childI = lookup.find(childId);
  if (parentI == lookup.end()) {
    cout << "The specified parent does not exist!\n";
    return;
  }
  if (childI == lookup.end()) {
    cout << "The specified child does not exist!\n";
    return;
  }

  Component *parent = parentI->second.get();
  Component *child = childI->second.get();

  if (child->getParent() != nullptr) {
    cout << "Child '" << childId << "' already belongs to '"
         << child->getParent()->getID() << "'.\n"
         << "Use the move command to move it.\n";
    return;
  }

  parent->add(child);
}

void ProjectRegistry::removeChild(const string &parentId,
                                  const string &childId) {
  auto parentI = lookup.find(parentId);
  auto childI = lookup.find(childId);
  if (parentI == lookup.end()) {
    cout << "The specified parent does not exist!\n";
    return;
  }
  if (childI == lookup.end()) {
    cout << "The specified child does not exist!\n";
    return;
  }

  Component *parent = parentI->second.get();
  Component *child = childI->second.get();

  parent->remove(child);
}

void ProjectRegistry::moveChild(const string &childId,
                                const string &newParentId) {
  auto parentI = lookup.find(newParentId);
  auto childI = lookup.find(childId);
  if (parentI == lookup.end()) {
    cout << "The specified parent does not exist!\n";
    return;
  }
  if (childI == lookup.end()) {
    cout << "The specified child does not exist!\n";
    return;
  }

  Component *parent = parentI->second.get();
  Component *child = childI->second.get();
  Component *oldParent = child->getParent();
  oldParent->remove(child);
  parent->add(child);
}

void ProjectRegistry::setState(const string &id, const string &newState) {
  auto targetI = lookup.find(id);
  if (targetI == lookup.end()) {
    cout << "The specified target does not exist!\n";
    return;
  }

  Component *target = targetI->second.get();
  target->setState(newState);
}

string ProjectRegistry::getState(const string &id) const {
  auto targetI = lookup.find(id);
  if (targetI == lookup.end()) {
    cout << "The specified target does not exist!\n";
    return nullptr;
  }
  Component *target = targetI->second.get();
  return target->getState();
}

void ProjectRegistry::decorateIssue(const string &issueId,
                                    unique_ptr<IssueDecorator> decorator) {
  auto targetI = lookup.find(issueId);
  if (targetI == lookup.end()) {
    cout << "Issue '" << issueId << "' does not exist.\n";
    return;
  }

  Component *oldIssue = targetI->second.get();
  Component *parent = oldIssue->getParent();

  targetI->second = std::move(decorator); // Replace

  if (parent) {
    parent->replaceChild(oldIssue, targetI->second.get());
  }
  cout << "Decorated issue '" << issueId << "'.\n";
}

vector<string> ProjectRegistry::getChildIds(const string &parentId) const {
  vector<string> returnValue;
  return returnValue;
}

void ProjectRegistry::printTree() const {}
