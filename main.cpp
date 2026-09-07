#include "Issue.h"
#include "Composite.h"
#include "DepthFirstIterator.h"
#include "UnresolvedIssueIterator.h"
#include "PriorityDecorator.h"
#include "TagDecorator.h"
#include <iostream>
#include <memory>

//static testing for states
int main() {

    Issue issue(
        "Fix login vulnerability",
        "ISS-101",
        "Repair authentication token validation"
    );

    std::cout << "\n--- ISSUE CREATED ---\n";
    std::cout
        << issue.getID()
        << " state: "
        << issue.getState()
        << "\n";


    // --------------------------------
    // INVALID transition
    // --------------------------------

    std::cout << "\n--- TRY INVALID ACTION ---\n";

    issue.approve();

    std::cout
        << "State remains: "
        << issue.getState()
        << "\n";


    // --------------------------------
    // Normal lifecycle
    // --------------------------------

    std::cout << "\n--- ASSIGN ISSUE ---\n";
    issue.assign();


    std::cout << "\n--- START WORK ---\n";
    issue.startWork();


    std::cout << "\n--- SUBMIT FOR REVIEW ---\n";
    issue.submitForReview();


    // --------------------------------
    // Reviewer rejects it
    // --------------------------------

    std::cout << "\n--- REVIEW REJECTED ---\n";
    issue.reject();


    // Developer fixes it.
    std::cout << "\n--- SUBMIT AGAIN ---\n";
    issue.submitForReview();


    // Reviewer approves.
    std::cout << "\n--- APPROVED ---\n";
    issue.approve();


    std::cout
        << "\nFinal state: "
        << issue.getState()
        << "\n";


    // --------------------------------
    // Reopen issue
    // --------------------------------

    std::cout << "\n--- REOPEN ISSUE ---\n";
    issue.reopen();

    std::cout
        << "New state: "
        << issue.getState()
        << "\n";


    // ================================
    // ITERATOR PATTERN DEMONSTRATION
    // ================================

    std::cout << "\n\n========== ITERATOR DEMONSTRATION ==========\n";

    // Create a nested project hierarchy.
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

    // Build the hierarchy:
    //
    // Project
    //   -> Backend Repository
    //       -> Authentication Feature
    //           -> Login Bug
    //           -> Token Bug

    project.add(&backend);
    backend.add(&authentication);
    authentication.add(&loginBug);
    authentication.add(&tokenBug);


    // ------------------------------------------------
    // DEPTH-FIRST TRAVERSAL
    // ------------------------------------------------

    std::cout << "\n--- DEPTH-FIRST TRAVERSAL ---\n";

    std::unique_ptr<ProjectIterator> depthIterator =
        project.createIterator();

    while (depthIterator->hasNext()) {
        Component* component = depthIterator->next();

        std::cout
            << component->getID()
            << " - "
            << component->getName()
            << "\n";
    }


    // ------------------------------------------------
    // UNRESOLVED WORK ITEMS
    // ------------------------------------------------

    std::cout << "\n--- UNRESOLVED WORK ITEMS ---\n";

    UnresolvedIssueIterator unresolvedIterator(&project);

    while (unresolvedIterator.hasNext()) {
        Component* component = unresolvedIterator.next();

        std::cout
            << component->getID()
            << " - "
            << component->getName()
            << " ["
            << component->getState()
            << "]\n";
    }


    // ------------------------------------------------
    // TWO INDEPENDENT ITERATORS
    // ------------------------------------------------

    std::cout << "\n--- TWO INDEPENDENT ITERATORS ---\n";

    std::unique_ptr<ProjectIterator> iteratorOne =
        project.createIterator();

    std::unique_ptr<ProjectIterator> iteratorTwo =
        project.createIterator();

    Component* firstFromOne = iteratorOne->next();
    Component* secondFromOne = iteratorOne->next();

    Component* firstFromTwo = iteratorTwo->next();

    std::cout
        << "Iterator 1 first:  "
        << firstFromOne->getName()
        << "\n";

    std::cout
        << "Iterator 1 second: "
        << secondFromOne->getName()
        << "\n";

    std::cout
        << "Iterator 2 first:  "
        << firstFromTwo->getName()
        << "\n";
    // ------------------------------------------------
    // SNAPSHOT BEHAVIOUR DURING MODIFICATION
    // ------------------------------------------------

    std::cout << "\n--- SNAPSHOT ITERATOR POLICY ---\n";

    // Create the iterator BEFORE changing the hierarchy.
    std::unique_ptr<ProjectIterator> snapshotIterator =
        project.createIterator();

    // Add a new issue after the iterator has already
    // captured its traversal.
    Issue newIssue(
        "Add Password Reset",
        "ISS-203",
        "Implement password reset functionality"
    );

    authentication.add(&newIssue);

    std::cout << "Existing iterator after hierarchy change:\n";

    while (snapshotIterator->hasNext()) {
        Component* component = snapshotIterator->next();

        std::cout
            << component->getID()
            << " - "
            << component->getName()
            << "\n";
    }

    std::cout << "\nNew iterator after hierarchy change:\n";

    std::unique_ptr<ProjectIterator> newIterator =
        project.createIterator();

    while (newIterator->hasNext()) {
        Component* component = newIterator->next();

        std::cout
            << component->getID()
            << " - "
            << component->getName()
            << "\n";
    }
    // ================================
    // DECORATOR PATTERN DEMONSTRATION
    // ================================

    std::cout << "\n\n========== DECORATOR DEMONSTRATION ==========\n";

    Issue securityIssue(
        "Fix Authentication Vulnerability",
        "ISS-301",
        "Repair authentication vulnerability"
    );

    std::cout << "\n--- ORIGINAL ISSUE ---\n";
    std::cout << "Description: "
              << securityIssue.getDescription()
              << "\n";

    // Add priority dynamically at runtime.
    PriorityDecorator priorityIssue(
        &securityIssue,
        "High"
    );

    std::cout << "\n--- PRIORITY DECORATOR ADDED ---\n";
    std::cout << "Description: "
              << priorityIssue.getDescription()
              << "\n";

    std::cout << "Has High priority: "
              << (priorityIssue.hasPriority("High") ? "Yes" : "No")
              << "\n";

    // Stack another decorator around the priority decorator.
    TagDecorator taggedIssue(
        &priorityIssue,
        "Security"
    );

    std::cout << "\n--- STACKED DECORATORS ---\n";
    std::cout << "Description: "
              << taggedIssue.getDescription()
              << "\n";

    std::cout << "Has High priority through decorator chain: "
              << (taggedIssue.hasPriority("High") ? "Yes" : "No")
              << "\n";

    // State operations still reach the underlying Issue.
    taggedIssue.setState("Assigned");

    std::cout << "State through decorated component: "
              << taggedIssue.getState()
              << "\n";

    std::cout << "Underlying issue state: "
              << securityIssue.getState()
              << "\n";
    return 0;
}