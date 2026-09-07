#include "Issue.h"
#include "Composite.h"
#include "DepthFirstIterator.h"
#include "UnresolvedIssueIterator.h"

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

    return 0;
}