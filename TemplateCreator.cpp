#include "TemplateCreator.h"
#include "ProjectRegistry.h"
#include <iostream>

using namespace std;

namespace {
void setDemoState(ProjectRegistry *registry, const string &id,
                  const string &target) {
    if (target == "Assigned" || target == "InProgress" ||
        target == "Review" || target == "Closed") {
        registry->setState(id, "Assigned");
    }
    if (target == "InProgress" || target == "Review" || target == "Closed") {
        registry->setState(id, "InProgress");
    }
    if (target == "Review" || target == "Closed") {
        registry->setState(id, "Review");
    }
    if (target == "Closed") {
        registry->setState(id, "Closed");
    }
}
}

void TemplateCreator::build(ProjectRegistry *registry) {
    if (!registry) {
        cerr << "Registry is null. Cannot build demo tree.\n";
        return;
    }

    cout << "Building demo TaskForge tree...\n";

    registry->createRepo(
        "TaskForge",
        "repo-1",
        "Main TaskForge repository",
        "John Doe",
        false,
        "C++"
    );

    registry->createProject(
        "Composite Pattern",
        "proj-1",
        "Sprint-1",
        40,
        "Implement the GoF Composite pattern"
    );

    registry->createProject(
        "State Pattern",
        "proj-2",
        "Sprint-1",
        30,
        "Implement the GoF State pattern"
    );

    registry->createProject(
        "Iterator Pattern",
        "proj-3",
        "Sprint-2",
        25,
        "Implement the GoF Iterator pattern"
    );

    registry->createProject(
        "Decorator Pattern",
        "proj-4",
        "Sprint-2",
        35,
        "Implement the GoF Decorator pattern"
    );

    registry->createIssue(
        "Design Component interface",
        "issue-1-1",
        "Create abstract Component class with virtual methods"
    );
    registry->createIssue(
        "Implement Leaf",
        "issue-1-2",
        "Create Issue class (leaf node)"
    );
    registry->createIssue(
        "Implement Composite",
        "issue-1-3",
        "Create Composite class with child management"
    );
    registry->createIssue(
        "Test Hierarchy",
        "issue-1-4",
        "Build and test 3-level structure"
    );

    registry->createIssue(
        "Create State interface",
        "issue-2-1",
        "Abstract State class with getName(), transitionTo()"
    );
    registry->createIssue(
        "Implement OpenState",
        "issue-2-2",
        "Initial state for issues"
    );
    registry->createIssue(
        "Implement AssignedState",
        "issue-2-3",
        "State when issue is assigned"
    );
    registry->createIssue(
        "Implement InProgressState",
        "issue-2-4",
        "State when work has started"
    );
    registry->createIssue(
        "Implement ClosedState",
        "issue-2-5",
        "State when work is complete"
    );

    registry->createIssue(
        "Create Iterator interface",
        "issue-3-1",
        "ProjectIterator abstract class"
    );
    registry->createIssue(
        "Implement DepthFirstIterator",
        "issue-3-2",
        "Traverse tree in pre-order"
    );
    registry->createIssue(
        "Implement BreadthFirstIterator",
        "issue-3-3",
        "Traverse tree level by level"
    );
    registry->createIssue(
        "Implement HighPriorityIterator",
        "issue-3-4",
        "Filter nodes by priority"
    );

    registry->createIssue(
        "Create Decorator interface",
        "issue-4-1",
        "IssueDecorator abstract class"
    );
    registry->createIssue(
        "Implement PriorityDecorator",
        "issue-4-2",
        "Add priority level to issues"
    );
    registry->createIssue(
        "Implement LabelDecorator",
        "issue-4-3",
        "Add category labels to issues"
    );
    registry->createIssue(
        "Test Stacking",
        "issue-4-4",
        "Demonstrate multiple decorators"
    );

    registry->addChild("repo-1", "proj-1");
    registry->addChild("repo-1", "proj-2");
    registry->addChild("repo-1", "proj-3");
    registry->addChild("repo-1", "proj-4");

    registry->addChild("proj-1", "issue-1-1");
    registry->addChild("proj-1", "issue-1-2");
    registry->addChild("proj-1", "issue-1-3");
    registry->addChild("proj-1", "issue-1-4");

    registry->addChild("proj-2", "issue-2-1");
    registry->addChild("proj-2", "issue-2-2");
    registry->addChild("proj-2", "issue-2-3");
    registry->addChild("proj-2", "issue-2-4");
    registry->addChild("proj-2", "issue-2-5");

    registry->addChild("proj-3", "issue-3-1");
    registry->addChild("proj-3", "issue-3-2");
    registry->addChild("proj-3", "issue-3-3");
    registry->addChild("proj-3", "issue-3-4");

    registry->addChild("proj-4", "issue-4-1");
    registry->addChild("proj-4", "issue-4-2");
    registry->addChild("proj-4", "issue-4-3");
    registry->addChild("proj-4", "issue-4-4");

    setDemoState(registry, "issue-1-1", "InProgress");
    setDemoState(registry, "issue-1-2", "Assigned");
    setDemoState(registry, "issue-1-3", "Open");
    setDemoState(registry, "issue-1-4", "Open");

    setDemoState(registry, "issue-2-1", "Closed");
    setDemoState(registry, "issue-2-2", "InProgress");
    setDemoState(registry, "issue-2-3", "Assigned");
    setDemoState(registry, "issue-2-4", "Open");
    setDemoState(registry, "issue-2-5", "Open");

    setDemoState(registry, "issue-3-1", "Closed");
    setDemoState(registry, "issue-3-2", "InProgress");
    setDemoState(registry, "issue-3-3", "Assigned");
    setDemoState(registry, "issue-3-4", "Open");

    setDemoState(registry, "issue-4-1", "Open");
    setDemoState(registry, "issue-4-2", "InProgress");
    setDemoState(registry, "issue-4-3", "Assigned");
    setDemoState(registry, "issue-4-4", "Open");

    cout << "Demo tree built successfully!\n";
}