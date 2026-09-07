#include "Issue.h"

#include <iostream>

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

    return 0;
}