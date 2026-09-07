
#include "ProjectRegistry.h"
#include "Component.h"
#include "Composite.h"
#include "Issue.h"
#include "Project.h"
#include "ProjectIterator.h"
#include "PriorityDecorator.h"
#include "Repo.h"
#include "TagDecorator.h"
#include "UnresolvedIssueIterator.h"
#include <iostream>
#include <memory>

using namespace std;


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

void ProjectRegistry::createProject(const string &name, const string &id,
                                    const string &deadline, int estimatedHours,
                                    const string &desc) {
  if (exists(id)) {
    cout << "Component with ID '" << id << "' already exists.\n";
    return;
  }

  lookup.emplace(id, unique_ptr<Component>(new Project(name, id, desc, deadline,
                                                       estimatedHours)));
}

void ProjectRegistry::createRepo(const string &name, const string &id,
                                 const string &desc, string owner,
                                 bool privateRepo, string language) {
  if (exists(id)) {
    cout << "Component with ID '" << id << "' already exists.\n";
    return;
  }

  lookup.emplace(id, unique_ptr<Component>(new Repo(name, id, desc, owner,
                                                    privateRepo, language)));
}

void ProjectRegistry::createIssue(const string &name, const string &id,
                                  const string &desc) {
  if (exists(id)) {
    cout << "Component with ID '" << id << "' already exists.\n";
    return;
  }

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
  Composite *compositeParent = dynamic_cast<Composite *>(parent);
  if (!compositeParent) {
    cout << "The specified parent cannot contain children!\n";
    return;
  }
  if (!oldParent) {
    cout << "The specified child is not attached to a parent!\n";
    return;
  }
  for (Component *ancestor = parent; ancestor != nullptr;
       ancestor = ancestor->getParent()) {
    if (ancestor == child) {
      cout << "Cannot move a component beneath itself or its descendant.\n";
      return;
    }
  }

  oldParent->remove(child);
  compositeParent->add(child);
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
    return "";
  }
  Component *target = targetI->second.get();
  return target->getState();
}

void ProjectRegistry::addPriority(const string &issueId,
                                  const string &level) {
  Component *issue = getComponent(issueId);
  if (!issue) {
    cout << "Issue '" << issueId << "' does not exist.\n";
    return;
  }

  unique_ptr<IssueDecorator> decorator(
      new PriorityDecorator(issue, level));
  decorateIssue(issueId, std::move(decorator));
}

void ProjectRegistry::addTag(const string &issueId, const string &tag) {
  Component *issue = getComponent(issueId);
  if (!issue) {
    cout << "Issue '" << issueId << "' does not exist.\n";
    return;
  }

  unique_ptr<IssueDecorator> decorator(new TagDecorator(issue, tag));
  decorateIssue(issueId, std::move(decorator));
}

bool ProjectRegistry::checkPriority(const string &issueId,
                                    const string &level) const {
  Component *issue = getComponent(issueId);
  return issue && issue->hasPriority(level);
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

  if (!decorator) {
    cout << "Decorator cannot be null.\n";
    return;
  }

  const string retainedId = issueId + "#wrapped#" +
                            to_string(decoratedComponents.size());
  decoratedComponents.emplace(retainedId, std::move(targetI->second));
  targetI->second = std::move(decorator);

  if (parent) {
    parent->replaceChild(oldIssue, targetI->second.get());
  }
  cout << "Decorated issue '" << issueId << "'.\n";
}

vector<string> ProjectRegistry::getChildIds(const string &parentId) const {
  vector<string> result;

    auto it = lookup.find(parentId);
    if (it == lookup.end()) {
        cout << "Parent '" << parentId << "' does not exist.\n";
        return result;
    }

    Component* parent = it->second.get();

    for (int index = 0; index < parent->getChildCount(); ++index) {
    Component* child = parent->getChild(index);
        if (child) {
            result.push_back(child->getID());
        }
    }

    return result;
}

void ProjectRegistry::printTree() const {
  cout << "\n=== TaskForge Tree ===\n";

    bool foundRoot = false;
    for (const auto& pair : lookup) {
        const Component* node = pair.second.get();
        if (node->getParent() == nullptr) {
            foundRoot = true;
            printNode(node, 0);
        }
    }

    if (!foundRoot) {
        cout << "The tree is empty.\n";
    }
}

void ProjectRegistry::printDepthFirst(const string &rootId) const {
  Component *root = getComponent(rootId);
  if (!root) {
    cout << "Component '" << rootId << "' does not exist.\n";
    return;
  }

  unique_ptr<ProjectIterator> iterator = root->createIterator();
  if (!iterator) {
    cout << "Component '" << rootId << "' cannot be traversed.\n";
    return;
  }

  cout << "\n=== Depth-First Traversal: " << rootId << " ===\n";
  while (iterator->hasNext()) {
    Component *component = iterator->next();
    if (component) {
      cout << component->getID()
           << " - " << component->getName()
           << " [" << component->getState() << "]\n";
    }
  }
}

void ProjectRegistry::printUnresolved(const string &rootId) const {
  Component *root = getComponent(rootId);
  if (!root) {
    cout << "Component '" << rootId << "' does not exist.\n";
    return;
  }

  UnresolvedIssueIterator iterator(root);
  cout << "\n=== Unresolved Issues: " << rootId << " ===\n";
  if (!iterator.hasNext()) {
    cout << "No unresolved issues.\n";
    return;
  }

  while (iterator.hasNext()) {
    Component *component = iterator.next();
    if (component) {
      cout << component->getID()
           << " - " << component->getName()
           << " [" << component->getState() << "]\n";
    }
  }
}

void ProjectRegistry::printNode(const Component* node, int depth) const {
    if (!node) return;

    for (int i = 0; i < depth; ++i) {
        cout << "  ";
    }

    // Print the node
    cout << "├── " << node->getName()
         << " (" << node->getID() << ")"
         << " [" << node->getState() << "]"
         << " - " << node->getDescription()
         << "\n";

    for (int index = 0; index < node->getChildCount(); ++index) {
        Component* child = node->getChild(index);
        if (child) {
            printNode(child, depth + 1);
        }
    }
}
