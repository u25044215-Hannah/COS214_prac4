#include "Issue.h"
#include "Composite.h"
#include "TemplateCreator.h"
#include "ProjectRegistry.h"
#include "Repo.h"
#include "UnresolvedIssueIterator.h"
#include "PriorityDecorator.h"
#include "TagDecorator.h"
#include <iostream>
#include <memory>
#include <utility>
#include <cassert>

using namespace std;

// ============================================================
// STATIC TESTS (Your Original Code)
// ============================================================

namespace {

void closeIssue(Issue &issue) {
    issue.assign();
    issue.startWork();
    issue.submitForReview();
    issue.approve();
}

void testRepo() {
    Repo privateRepo("Private Repo", "repo-private", "Source code",
                     "Alice", true, "C++");
    Repo publicRepo("Public Repo", "repo-public", "Open source",
                    "Bob", false, "C++");

    assert(privateRepo.getName() == "Private Repo");
    assert(privateRepo.getID() == "repo-private");
    assert(privateRepo.getDescription() ==
           "[Private] [Alice] Source code (C++)");
    assert(publicRepo.getDescription() ==
           "[Public] [Bob] Open source (C++)");
    assert(privateRepo.getChildCount() == 0);
    assert(privateRepo.getState() == "Open");
}

void testComponentDefaults() {
    Issue issue("Leaf", "leaf", "Leaf description");

    assert(issue.getName() == "Leaf");
    assert(issue.getID() == "leaf");
    assert(issue.getDescription() == "Leaf description");
    assert(issue.getChild(0) == nullptr);
    assert(issue.getChildCount() == 0);
    assert(!issue.createIterator());
    assert(!issue.hasPriority("High"));
    assert(issue.getParent() == nullptr);

    issue.add(nullptr);
    issue.remove(nullptr);
    issue.replaceChild(nullptr, nullptr);

    assert(issue.isOpen());
    assert(!issue.isAssigned());
    assert(!issue.isInProgress());
    assert(!issue.isClosed());
    issue.assign();
    assert(issue.isOpen());
    assert(issue.isAssigned());
    issue.startWork();
    assert(issue.isInProgress());
    issue.submitForReview();
    issue.approve();
    assert(issue.isClosed());
}

void testUnsafeInputs() {
    PriorityDecorator nullPriority(nullptr, "High");
    TagDecorator nullTag(nullptr, "Security");

    assert(nullPriority.getState() == "Unknown");
    assert(!nullPriority.hasPriority("Low"));
    assert(nullPriority.getDescription() == " [Priority: High]");
    nullPriority.setState("Assigned");
    assert(nullTag.getDescription() == " [Tag: Security]");

    Composite root("Root", "root");
    Composite child("Child", "child");
    root.add(&child);
    child.add(&root);
    assert(root.getChildCount() == 1);
    assert(child.getChildCount() == 0);
    assert(root.createIterator()->next() == &root);
}

void testComposite() {
    Composite empty("Empty", "empty");
    Composite parent("Parent", "parent");
    Composite child("Child", "child");
    Issue open("Open", "open");
    Issue assigned("Assigned", "assigned");
    Issue inProgress("In progress", "in-progress");
    Issue review("Review", "review");
    Issue closed("Closed", "closed");

    assert(empty.getState() == "Open");
    assert(empty.getChildCount() == 0);
    assert(empty.getChild(-1) == nullptr);
    assert(empty.getChild(0) == nullptr);

    parent.add(nullptr);
    parent.add(&parent);
    parent.add(&child);
    parent.add(&child);
    child.add(&parent);
    assert(parent.getChildCount() == 1);
    assert(child.getParent() == &parent);

    child.add(&open);
    child.add(&assigned);
    child.add(&inProgress);
    child.add(&review);
    child.add(&closed);
    review.assign();
    review.startWork();
    review.submitForReview();
    assert(parent.getState() == "Review");

    child.remove(&open);
    child.remove(&open);
    child.remove(nullptr);
    assert(child.getChildCount() == 4);

    Composite replacement("Replacement", "replacement");
    child.replaceChild(&assigned, &replacement);
    assert(replacement.getParent() == &child);
    assert(assigned.getParent() == nullptr);
    assert(child.getChild(0) == &replacement);

    Composite assignedOnly("Assigned only", "assigned-only");
    Issue assignedIssue("Assigned", "assigned-only-issue");
    assignedIssue.assign();
    assignedOnly.add(&assignedIssue);
    assert(assignedOnly.getState() == "Assigned");

    Composite progressOnly("Progress only", "progress-only");
    Issue progressIssue("Progress", "progress-only-issue");
    progressIssue.assign();
    progressIssue.startWork();
    progressOnly.add(&progressIssue);
    assert(progressOnly.getState() == "InProgress");

    Composite closedOnly("Closed only", "closed-only");
    Issue closedIssue("Closed", "closed-only-issue");
    closeIssue(closedIssue);
    closedOnly.add(&closedIssue);
    assert(closedOnly.getState() == "Closed");

    Composite openOnly("Open only", "open-only");
    Issue openOnlyIssue("Open", "open-only-issue");
    openOnly.add(&openOnlyIssue);
    assert(openOnly.getState() == "Open");
    openOnly.setState("Assigned");
    assert(openOnlyIssue.isAssigned());

    UnresolvedIssueIterator noRoot(nullptr);
    assert(!noRoot.hasNext());
    assert(noRoot.next() == nullptr);

    Composite unresolvedRoot("Unresolved root", "unresolved-root");
    Issue unresolvedOpen("Unresolved", "unresolved-open");
    Issue unresolvedClosed("Resolved", "unresolved-closed");
    closeIssue(unresolvedClosed);
    unresolvedRoot.add(&unresolvedOpen);
    unresolvedRoot.add(&unresolvedClosed);
    UnresolvedIssueIterator unresolved(&unresolvedRoot);
    assert(unresolved.hasNext());
    assert(unresolved.next() == &unresolvedOpen);
    assert(!unresolved.hasNext());
    unresolved.reset();
    assert(unresolved.next() == &unresolvedOpen);

    std::unique_ptr<ProjectIterator> iterator = parent.createIterator();
    assert(iterator);
    assert(iterator->next() == &parent);
    iterator->reset();
    assert(iterator->hasNext());
}

void testProjectRegistry() {
    ProjectRegistry registry;
    registry.createRepo("TaskForge", "repo-1", "Main repo",
                        "Owner", false, "C++");
    registry.createProject("Backend", "project-1", "Sprint work",
                           10, "Sprint 1");
    registry.createIssue("Fix bug", "issue-1", "A bug");
    registry.createIssue("Duplicate", "issue-1", "Ignored");

    assert(registry.exists("repo-1"));
    assert(!registry.exists("missing"));
    assert(registry.getComponent("missing") == nullptr);
    assert(registry.getComponent("issue-1")->getName() == "Fix bug");
    assert(registry.getState("missing").empty());

    registry.addChild("repo-1", "project-1");
    registry.addChild("project-1", "issue-1");
    assert(registry.getChildIds("repo-1").size() == 1);
    assert(registry.getChildIds("repo-1")[0] == "project-1");
    assert(registry.getChildIds("missing").empty());

    registry.addChild("missing", "issue-1");
    registry.addChild("repo-1", "missing");
    registry.addChild("repo-1", "issue-1");

    registry.createIssue("Loose issue", "loose", "Not attached");
    registry.moveChild("loose", "repo-1");
    registry.moveChild("missing", "repo-1");
    registry.moveChild("issue-1", "missing");
    registry.moveChild("repo-1", "issue-1");

    registry.removeChild("repo-1", "project-1");
    registry.removeChild("repo-1", "missing");
    registry.addChild("repo-1", "project-1");
    assert(registry.getComponent("project-1")->getParent() ==
           registry.getComponent("repo-1"));

    registry.setState("missing", "Assigned");
    registry.setState("issue-1", "Assigned");
    assert(registry.getState("issue-1") == "Assigned");

    registry.decorateIssue("missing", nullptr);
    registry.decorateIssue("issue-1", nullptr);
    registry.addChild("project-1", "issue-1");
    std::unique_ptr<IssueDecorator> decorator(
        new PriorityDecorator(registry.getComponent("issue-1"), "High"));
    registry.decorateIssue("issue-1", std::move(decorator));
    assert(registry.getComponent("issue-1")->hasPriority("High"));
    assert(registry.getComponent("project-1")->getChild(0) ==
           registry.getComponent("issue-1"));
    registry.printTree();
}

void runStaticTests() {
    std::cout << "\n========== STATIC TESTS ==========\n";
    testComponentDefaults();
    testUnsafeInputs();
    testRepo();
    testComposite();
    testProjectRegistry();
}

} // namespace

// ============================================================
// STATE LIFECYCLE DEMONSTRATION
// ============================================================

void demonstrateStateLifecycle() {
    Issue issue(
        "Fix login vulnerability",
        "ISS-101",
        "Repair authentication token validation"
    );

    std::cout << "\n--- ISSUE CREATED ---\n";
    std::cout << issue.getID() << " state: " << issue.getState() << "\n";

    std::cout << "\n--- TRY INVALID ACTION ---\n";
    issue.approve();
    std::cout << "State remains: " << issue.getState() << "\n";

    std::cout << "\n--- ASSIGN ISSUE ---\n";
    issue.assign();

    std::cout << "\n--- START WORK ---\n";
    issue.startWork();

    std::cout << "\n--- SUBMIT FOR REVIEW ---\n";
    issue.submitForReview();

    std::cout << "\n--- REVIEW REJECTED ---\n";
    issue.reject();

    std::cout << "\n--- SUBMIT AGAIN ---\n";
    issue.submitForReview();

    std::cout << "\n--- APPROVED ---\n";
    issue.approve();

    std::cout << "\nFinal state: " << issue.getState() << "\n";

    std::cout << "\n--- REOPEN ISSUE ---\n";
    issue.reopen();
    std::cout << "New state: " << issue.getState() << "\n";
}

// ============================================================
// ITERATOR DEMONSTRATION
// ============================================================

void demonstrateIterators() {
    std::cout << "\n\n========== ITERATOR DEMONSTRATION ==========\n";

    Composite project(
        "TaskForge Project",
        "PROJ-001",
        "Root software project"
    );

    Composite backend(
        "Backend Repository",
        "REP-001",
        "Backend development work"
    );

    Composite authentication(
        "Authentication Feature",
        "FEAT-001",
        "Authentication functionality"
    );

    Issue loginBug(
        "Fix Login Bug",
        "ISS-201",
        "Fix incorrect login behaviour"
    );

    Issue tokenBug(
        "Fix Token Validation",
        "ISS-202",
        "Repair token validation"
    );

    project.add(&backend);
    backend.add(&authentication);
    authentication.add(&loginBug);
    authentication.add(&tokenBug);

    std::cout << "\n--- DEPTH-FIRST TRAVERSAL ---\n";
    std::unique_ptr<ProjectIterator> depthIterator =
        project.createIterator();

    while (depthIterator->hasNext()) {
        Component* component = depthIterator->next();
        std::cout << component->getID() << " - " << component->getName() << "\n";
    }

    std::cout << "\n--- UNRESOLVED WORK ITEMS ---\n";
    UnresolvedIssueIterator unresolvedIterator(&project);

    while (unresolvedIterator.hasNext()) {
        Component* component = unresolvedIterator.next();
        std::cout << component->getID() << " - " << component->getName()
                  << " [" << component->getState() << "]\n";
    }

    std::cout << "\n--- TWO INDEPENDENT ITERATORS ---\n";
    std::unique_ptr<ProjectIterator> iteratorOne =
        project.createIterator();

    std::unique_ptr<ProjectIterator> iteratorTwo =
        project.createIterator();

    Component* firstFromOne = iteratorOne->next();
    Component* secondFromOne = iteratorOne->next();

    Component* firstFromTwo = iteratorTwo->next();

    std::cout << "Iterator 1 first:  " << firstFromOne->getName() << "\n";
    std::cout << "Iterator 1 second: " << secondFromOne->getName() << "\n";
    std::cout << "Iterator 2 first:  " << firstFromTwo->getName() << "\n";

    std::cout << "\n--- SNAPSHOT ITERATOR POLICY ---\n";

    std::unique_ptr<ProjectIterator> snapshotIterator =
        project.createIterator();

    Issue newIssue(
        "Add Password Reset",
        "ISS-203",
        "Implement password reset functionality"
    );

    authentication.add(&newIssue);

    std::cout << "Existing iterator after hierarchy change:\n";
    while (snapshotIterator->hasNext()) {
        Component* component = snapshotIterator->next();
        std::cout << component->getID() << " - " << component->getName() << "\n";
    }

    std::cout << "\nNew iterator after hierarchy change:\n";
    std::unique_ptr<ProjectIterator> newIterator =
        project.createIterator();

    while (newIterator->hasNext()) {
        Component* component = newIterator->next();
        std::cout << component->getID() << " - " << component->getName() << "\n";
    }
}

// ============================================================
// DECORATOR DEMONSTRATION
// ============================================================

void demonstrateDecorators() {
    std::cout << "\n\n========== DECORATOR DEMONSTRATION ==========\n";

    Issue securityIssue(
        "Fix Authentication Vulnerability",
        "ISS-301",
        "Repair authentication vulnerability"
    );

    std::cout << "\n--- ORIGINAL ISSUE ---\n";
    std::cout << "Description: " << securityIssue.getDescription() << "\n";

    PriorityDecorator priorityIssue(
        &securityIssue,
        "High"
    );

    std::cout << "\n--- PRIORITY DECORATOR ADDED ---\n";
    std::cout << "Description: " << priorityIssue.getDescription() << "\n";
    std::cout << "Has High priority: "
              << (priorityIssue.hasPriority("High") ? "Yes" : "No") << "\n";

    TagDecorator taggedIssue(
        &priorityIssue,
        "Security"
    );

    std::cout << "\n--- STACKED DECORATORS ---\n";
    std::cout << "Description: " << taggedIssue.getDescription() << "\n";
    std::cout << "Has High priority through decorator chain: "
              << (taggedIssue.hasPriority("High") ? "Yes" : "No") << "\n";

    taggedIssue.setState("Assigned");

    std::cout << "State through decorated component: "
              << taggedIssue.getState() << "\n";
    std::cout << "Underlying issue state: "
              << securityIssue.getState() << "\n";
}

// ============================================================
// PROJECT REGISTRY DEMONSTRATION
// ============================================================

void demonstrateRegistry() {
    std::cout << "\n========== PROJECT REGISTRY DEMONSTRATION ==========\n";
    TemplateCreator creator;
    ProjectRegistry registry;
    creator.build(&registry);
    registry.printTree();
}

// ============================================================
// MAIN
// ============================================================

int main() {
    runStaticTests();

    demonstrateStateLifecycle();
    demonstrateIterators();
    demonstrateDecorators();
    demonstrateRegistry();

    return 0;
}