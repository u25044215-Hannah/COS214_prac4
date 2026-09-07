
#include "ProjectRegistry.h"
#include "Component.h"
#include "Issue.h"
#include "Project.h"
#include "Repo.h"
#include <memory>

using namespace std;

ProjectRegistry::ProjectRegistry(unique_ptr<TemplateCreator> creator) {
  this->creator = std::move(creator);
}

Component *ProjectRegistry::getComponent(const string &id) const {
  auto it = lookup.find(id);
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

void ProjectRegistry::addChild(const string &parentId, const string &childId) {}

void ProjectRegistry::removeChild(const string &parentId,
                                  const string &childId) {}

void ProjectRegistry::moveChild(const string &childId,
                                const string &newParentId) {}

void ProjectRegistry::setState(const string &id, const string &newState) {}

string ProjectRegistry::getState(const string &id) const {
  string returnValue;
  return returnValue;
}

void ProjectRegistry::decorateIssue(const string &issueId,
                                    unique_ptr<IssueDecorator> decorator) {}

vector<string> ProjectRegistry::getChildIds(const string &parentId) const {
  vector<string> returnValue;
  return returnValue;
}

void ProjectRegistry::printTree() const {}
