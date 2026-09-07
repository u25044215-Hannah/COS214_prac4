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
#include <cassert>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

/*using namespace std;

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
    registry.addPriority("issue-1", "High");
    registry.addTag("issue-1", "Security");
    assert(registry.checkPriority("issue-1", "High"));
    assert(!registry.checkPriority("issue-1", "Low"));
    assert(!registry.checkPriority("missing", "High"));
    assert(registry.getComponent("project-1")->getChild(0) ==
           registry.getComponent("issue-1"));
    registry.printTree();
    registry.printDepthFirst("repo-1");
    registry.printUnresolved("repo-1");
    registry.printDepthFirst("missing");
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
    registry.printDepthFirst("repo-1");
    registry.printUnresolved("repo-1");
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
} */


using namespace std;

namespace {

const string ROOT_ID = "repo-taskforge";
int demoCounter = 1;

void line(char ch = '=', int width = 72) {
    cout << string(width, ch) << "\n";
}

void title(const string &text) {
    cout << "\n";
    line('=');
    cout << text << "\n";
    line('=');
}

void section(const string &text) {
    cout << "\n";
    line('-');
    cout << text << "\n";
    line('-');
}

void pauseDemo() {
    cout << "\nPress ENTER to continue...";
    string dummy;
    getline(cin, dummy);
}

string readLine(const string &prompt) {
    cout << prompt;
    string value;
    getline(cin, value);
    return value;
}

int readInt(const string &prompt, int minValue, int maxValue) {
    while (true) {
        string text = readLine(prompt);
        stringstream ss(text);
        int value = 0;
        char extra = '\0';

        if ((ss >> value) && !(ss >> extra) &&
            value >= minValue && value <= maxValue) {
            return value;
        }

        cout << "Please enter a number from " << minValue
             << " to " << maxValue << ".\n";
    }
}

bool readYesNo(const string &prompt) {
    while (true) {
        string answer = readLine(prompt + " (y/n): ");
        if (answer == "y" || answer == "Y" || answer == "yes" || answer == "Yes") {
            return true;
        }
        if (answer == "n" || answer == "N" || answer == "no" || answer == "No") {
            return false;
        }
        cout << "Please enter y or n.\n";
    }
}

string newDemoId(const string &prefix) {
    return prefix + "-" + to_string(demoCounter++);
}

void requestState(ProjectRegistry &registry,
                  const string &id,
                  const string &target,
                  const string &actionLabel) {
    Component *component = registry.getComponent(id);
    if (!component) {
        cout << "Component '" << id << "' does not exist.\n";
        return;
    }

    string before = component->getState();
    cout << "\nAction: " << actionLabel << "\n";
    cout << "Before: " << before << "\n";
    cout << "Requested target: " << target << "\n";

    registry.setState(id, target);

    Component *afterComponent = registry.getComponent(id);
    string after = afterComponent ? afterComponent->getState() : "Unknown";
    cout << "After:  " << after << "\n";

    if (before == after && before != target) {
        cout << "Result: transition rejected by the current State object.\n";
    } else if (after == target) {
        cout << "Result: valid transition completed.\n";
    }
}

void setInitialState(ProjectRegistry &registry,
                     const string &id,
                     const string &target) {
    if (target == "Assigned" || target == "InProgress" ||
        target == "Review" || target == "Closed") {
        registry.setState(id, "Assigned");
    }
    if (target == "InProgress" || target == "Review" || target == "Closed") {
        registry.setState(id, "InProgress");
    }
    if (target == "Review" || target == "Closed") {
        registry.setState(id, "Review");
    }
    if (target == "Closed") {
        registry.setState(id, "Closed");
    }
}

unique_ptr<ProjectRegistry> buildDemoRegistry() {
    unique_ptr<ProjectRegistry> registry(new ProjectRegistry());

    // Suppress setup messages so the demo starts with a clean screen.
    ostringstream hiddenOutput;
    streambuf *oldBuffer = cout.rdbuf(hiddenOutput.rdbuf());

    registry->createRepo(
        "TaskForge",
        ROOT_ID,
        "GitHub-style work management repository",
        "COS214 Team",
        false,
        "C++11"
    );

    registry->createProject(
        "Release 2.0",
        "project-release",
        "8 September 2026",
        80,
        "Main release milestone"
    );

    registry->createProject(
        "Authentication Feature",
        "feature-auth",
        "6 September 2026",
        32,
        "Login, password and token work"
    );

    registry->createProject(
        "User Profile Feature",
        "feature-profile",
        "7 September 2026",
        24,
        "Profile-related work"
    );

    registry->createProject(
        "Maintenance Sprint",
        "project-maintenance",
        "8 September 2026",
        30,
        "Infrastructure and maintenance work"
    );

    registry->createProject(
        "Build and CI Feature",
        "feature-ci",
        "7 September 2026",
        14,
        "Build tooling and CI work"
    );

    registry->createIssue(
        "Fix login validation",
        "ISS-101",
        "Correct validation when a user signs in"
    );
    registry->createIssue(
        "Add password reset",
        "ISS-102",
        "Implement the password reset workflow"
    );
    registry->createIssue(
        "Fix token vulnerability",
        "ISS-103",
        "Repair authentication token validation"
    );
    registry->createIssue(
        "Add avatar upload",
        "ISS-104",
        "Allow users to upload a profile image"
    );
    registry->createIssue(
        "Cache profile requests",
        "ISS-105",
        "Cache profile API responses"
    );
    registry->createIssue(
        "Repair Docker build",
        "ISS-201",
        "Fix the C++11 Docker build"
    );
    registry->createIssue(
        "Remove compiler warnings",
        "ISS-202",
        "Clean up compiler warnings"
    );

    // Three levels below the repository root:
    // Repo -> Project -> nested Project/Feature -> Issue.
    registry->addChild(ROOT_ID, "project-release");
    registry->addChild(ROOT_ID, "project-maintenance");

    registry->addChild("project-release", "feature-auth");
    registry->addChild("project-release", "feature-profile");
    registry->addChild("project-maintenance", "feature-ci");

    registry->addChild("feature-auth", "ISS-101");
    registry->addChild("feature-auth", "ISS-102");
    registry->addChild("feature-auth", "ISS-103");
    registry->addChild("feature-profile", "ISS-104");
    registry->addChild("feature-profile", "ISS-105");
    registry->addChild("feature-ci", "ISS-201");
    registry->addChild("feature-ci", "ISS-202");

    // Give the demo tree a useful mixture of states.
    setInitialState(*registry, "ISS-101", "Assigned");
    setInitialState(*registry, "ISS-103", "InProgress");
    setInitialState(*registry, "ISS-105", "Closed");
    setInitialState(*registry, "ISS-201", "Review");

    cout.rdbuf(oldBuffer);
    return registry;
}

void printPatternMap() {
    title("HOW THE FOUR REQUIRED PATTERNS FIT TOGETHER");
    cout <<
        "ProjectRegistry owns all Components\n"
        "          |\n"
        "          v\n"
        "       Component  <------------------------------+\n"
        "       /      \\                                |\n"
        "    Issue    Composite                           |\n"
        "   (Leaf)   /    |     \\                       |\n"
        "           Repo Project nested Project           |\n"
        "                                                  |\n"
        "STATE: Issue delegates lifecycle rules to State   |\n"
        "       Open -> Assigned -> InProgress -> Review    |\n"
        "                              |         |           |\n"
        "                              +<-reject-+           |\n"
        "                                        -> Closed  |\n"
        "                                                  |\n"
        "DECORATOR: PriorityDecorator / TagDecorator ------+\n"
        "           wrap an Issue but keep Component interface\n"
        "\n"
        "ITERATOR: DepthFirstIterator and UnresolvedIssueIterator\n"
        "          traverse the same Composite hierarchy independently.\n";

    cout << "\nDemo sentence: The Composite builds the tree, State controls an "
            "Issue's lifecycle, Decorator adds responsibilities at runtime, "
            "and Iterator traverses the hierarchy without exposing its vector.\n";
}

void printComponentDetails(ProjectRegistry &registry, const string &id) {
    Component *component = registry.getComponent(id);
    if (!component) {
        cout << "Component '" << id << "' does not exist.\n";
        return;
    }

    section("COMPONENT DETAILS");
    cout << "ID:          " << component->getID() << "\n";
    cout << "Name:        " << component->getName() << "\n";
    cout << "Description: " << component->getDescription() << "\n";
    cout << "State:       " << component->getState() << "\n";
    cout << "Children:    " << component->getChildCount() << "\n";
    cout << "Parent:      "
         << (component->getParent() ? component->getParent()->getID() : "<none>")
         << "\n";
    cout << "High priority: "
         << (component->hasPriority("High") ? "Yes" : "No") << "\n";
}

void showChildren(ProjectRegistry &registry, const string &id) {
    vector<string> children = registry.getChildIds(id);
    if (children.empty()) {
        cout << "No children found.\n";
        return;
    }

    cout << "Children of '" << id << "':\n";
    for (size_t i = 0; i < children.size(); ++i) {
        Component *child = registry.getComponent(children[i]);
        cout << "  " << (i + 1) << ". " << children[i];
        if (child) {
            cout << " - " << child->getName()
                 << " [" << child->getState() << "]";
        }
        cout << "\n";
    }
}

vector<string> collectTraversal(ProjectIterator *iterator) {
    vector<string> result;
    if (!iterator) {
        return result;
    }

    while (iterator->hasNext()) {
        Component *component = iterator->next();
        if (component) {
            result.push_back(component->getID());
        }
    }
    return result;
}

bool containsId(const vector<string> &items, const string &id) {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i] == id) {
            return true;
        }
    }
    return false;
}

void printIdList(const vector<string> &ids) {
    for (size_t i = 0; i < ids.size(); ++i) {
        cout << "  " << (i + 1) << ". " << ids[i] << "\n";
    }
}

void demonstrateIndependentIterators(ProjectRegistry &registry) {
    section("ITERATOR TEST: TWO INDEPENDENT TRAVERSALS");

    Component *root = registry.getComponent(ROOT_ID);
    if (!root) {
        cout << "Root not found.\n";
        return;
    }

    unique_ptr<ProjectIterator> first = root->createIterator();
    unique_ptr<ProjectIterator> second = root->createIterator();

    if (!first || !second) {
        cout << "Root cannot create an iterator.\n";
        return;
    }

    Component *a1 = first->next();
    Component *a2 = first->next();
    Component *b1 = second->next();

    cout << "Iterator A advanced twice:\n";
    cout << "  first item  = " << (a1 ? a1->getID() : "<none>") << "\n";
    cout << "  second item = " << (a2 ? a2->getID() : "<none>") << "\n";
    cout << "Iterator B advanced once:\n";
    cout << "  first item  = " << (b1 ? b1->getID() : "<none>") << "\n";
    cout << "\nConclusion: advancing A did not change B's position.\n";
}

void demonstrateSnapshot(ProjectRegistry &registry) {
    section("ITERATOR TEST: SNAPSHOT POLICY DURING STRUCTURAL CHANGE");

    Component *root = registry.getComponent(ROOT_ID);
    if (!root) {
        cout << "Root not found.\n";
        return;
    }

    unique_ptr<ProjectIterator> oldIterator = root->createIterator();
    if (!oldIterator) {
        cout << "Root cannot create an iterator.\n";
        return;
    }

    const string newId = newDemoId("ISS-SNAPSHOT");
    registry.createIssue(
        "Issue added during traversal",
        newId,
        "Created after an iterator snapshot already exists"
    );
    registry.addChild("feature-ci", newId);

    vector<string> oldItems = collectTraversal(oldIterator.get());

    unique_ptr<ProjectIterator> freshIterator = root->createIterator();
    vector<string> newItems = collectTraversal(freshIterator.get());

    cout << "New structure item: " << newId << "\n";
    cout << "Existing iterator sees it? "
         << (containsId(oldItems, newId) ? "YES" : "NO") << "\n";
    cout << "Fresh iterator sees it?    "
         << (containsId(newItems, newId) ? "YES" : "NO") << "\n";
    cout << "\nPolicy: iterators keep a snapshot of traversal order at creation time.\n";
}

void runStateLifecycleDemo(ProjectRegistry &registry, const string &id) {
    section("STATE PATTERN: COMPLETE ISSUE LIFECYCLE");

    if (!registry.exists(id)) {
        cout << "Issue '" << id << "' does not exist.\n";
        return;
    }

    cout << "Using issue: " << id << "\n";
    cout << "Starting state: " << registry.getState(id) << "\n";

    requestState(registry, id, "Closed",
                 "Try to close immediately (deliberately invalid)");
    requestState(registry, id, "Assigned", "Assign issue");
    requestState(registry, id, "InProgress", "Start work");
    requestState(registry, id, "Review", "Submit for review");
    requestState(registry, id, "InProgress", "Reviewer rejects work");
    requestState(registry, id, "Review", "Submit corrected work");
    requestState(registry, id, "Closed", "Reviewer approves work");
    requestState(registry, id, "Open", "Reopen the closed issue");

    cout << "\nState participant mapping:\n"
         << "  Context        = Issue\n"
         << "  State          = State\n"
         << "  ConcreteState  = OpenState, AssignedState, InProgressState, "
            "ReviewState, ClosedState\n";
}

void runDecoratorStackDemo(ProjectRegistry &registry, const string &id) {
    section("DECORATOR PATTERN: RUNTIME + STACKED RESPONSIBILITIES");

    Component *before = registry.getComponent(id);
    if (!before) {
        cout << "Issue '" << id << "' does not exist.\n";
        return;
    }

    cout << "Before decoration:\n";
    cout << "  " << before->getDescription() << "\n";

    registry.addPriority(id, "High");
    registry.addTag(id, "Security");

    Component *after = registry.getComponent(id);
    cout << "\nAfter stacking PriorityDecorator + TagDecorator:\n";
    cout << "  " << (after ? after->getDescription() : "<missing>") << "\n";
    cout << "  High priority? "
         << (registry.checkPriority(id, "High") ? "Yes" : "No") << "\n";
    cout << "  Interface is still Component*: "
         << (after ? "Yes" : "No") << "\n";

    if (after) {
        cout << "\nNormal behaviour still works through the decorated object.\n";
        cout << "Current state: " << after->getState() << "\n";
    }
}

void guidedDemo(unique_ptr<ProjectRegistry> &registry) {
    registry = buildDemoRegistry();

    title("GUIDED FULL PRACTICAL DEMO");
    cout << "This path is designed for the tutor demo.\n"
         << "It shows the four GoF patterns as ONE collaborating system.\n";
    pauseDemo();

    section("1/7 - COMPOSITE: RECURSIVE PART-WHOLE HIERARCHY");
    cout << "The same Component abstraction represents Repo, Project and Issue.\n"
         << "The hierarchy contains at least three levels below the repository:\n"
         << "Repo -> Project -> nested Project/Feature -> Issue.\n";
    registry->printTree();
    cout << "\nAggregate root state: " << registry->getState(ROOT_ID) << "\n";
    pauseDemo();

    section("2/7 - STATE: BEHAVIOUR CHANGES WITH ISSUE LIFECYCLE");
    cout << "We use ISS-102, which starts Open.\n"
         << "First we attempt an invalid transition, then follow valid transitions.\n";
    runStateLifecycleDemo(*registry, "ISS-102");
    pauseDemo();

    section("3/7 - DECORATOR: ADD RESPONSIBILITIES AT RUNTIME");
    cout << "We decorate ISS-103 without changing its concrete Issue class.\n";
    runDecoratorStackDemo(*registry, "ISS-103");
    cout << "\nNow submit the decorated issue for review:\n";
    requestState(*registry, "ISS-103", "Review",
                 "State transition through decorated Component");
    pauseDemo();

    section("4/7 - ITERATOR: TWO MEANINGFUL TRAVERSALS");
    cout << "Traversal A visits the complete hierarchy in depth-first order.\n";
    registry->printDepthFirst(ROOT_ID);
    cout << "\nTraversal B visits only unresolved leaf work items.\n";
    registry->printUnresolved(ROOT_ID);
    pauseDemo();

    section("5/7 - ITERATOR: INDEPENDENT TRAVERSALS");
    demonstrateIndependentIterators(*registry);
    pauseDemo();

    section("6/7 - RUNTIME STRUCTURAL CHANGE + SNAPSHOT POLICY");
    demonstrateSnapshot(*registry);

    Component *moving = registry->getComponent("ISS-104");
    cout << "\nNow move ISS-104 between groups at runtime.\n";
    cout << "Before parent: "
         << (moving && moving->getParent() ? moving->getParent()->getID() : "<none>")
         << "\n";
    registry->moveChild("ISS-104", "feature-auth");
    moving = registry->getComponent("ISS-104");
    cout << "After parent:  "
         << (moving && moving->getParent() ? moving->getParent()->getID() : "<none>")
         << "\n";
    pauseDemo();

    section("7/7 - FINAL INTEGRATED VIEW");
    registry->printTree();
    cout << "\nWhat this demonstrated:\n"
         << "  [Composite] recursive hierarchy + uniform Component interface\n"
         << "  [State]     valid/invalid lifecycle behaviour\n"
         << "  [Decorator] runtime and stacked responsibilities\n"
         << "  [Iterator]  full/filtered traversal, independent iterators, snapshot policy\n"
         << "  [Runtime]   structural and behavioural changes\n";
    printPatternMap();
}

void compositeMenu(ProjectRegistry &registry) {
    while (true) {
        title("COMPOSITE / HIERARCHY MENU");
        cout << "1. Print complete hierarchy\n"
             << "2. Inspect one component\n"
             << "3. List direct children\n"
             << "4. Show aggregate state of a group\n"
             << "0. Back\n";

        int choice = readInt("Choice: ", 0, 4);
        if (choice == 0) return;

        if (choice == 1) {
            registry.printTree();
        } else if (choice == 2) {
            printComponentDetails(registry, readLine("Component ID: "));
        } else if (choice == 3) {
            showChildren(registry, readLine("Parent ID: "));
        } else if (choice == 4) {
            string id = readLine("Group ID (e.g. repo-taskforge, project-release): ");
            Component *group = registry.getComponent(id);
            if (group) {
                cout << "Aggregate state of " << id << " = "
                     << group->getState() << "\n";
                cout << "The Composite computes its state from its children.\n";
            } else {
                cout << "Component not found.\n";
            }
        }
        pauseDemo();
    }
}

void stateMenu(ProjectRegistry &registry) {
    while (true) {
        title("STATE PATTERN MENU");
        cout << "Suggested issues: ISS-101, ISS-102, ISS-103, ISS-104, ISS-201\n\n"
             << "1. Inspect current state\n"
             << "2. Assign       -> Assigned\n"
             << "3. Start work   -> InProgress\n"
             << "4. Submit review-> Review\n"
             << "5. Approve      -> Closed\n"
             << "6. Reject       -> InProgress\n"
             << "7. Reopen       -> Open\n"
             << "8. Request any state (useful for invalid-transition test)\n"
             << "9. Create a fresh issue and run the complete lifecycle demo\n"
             << "0. Back\n";

        int choice = readInt("Choice: ", 0, 9);
        if (choice == 0) return;

        if (choice == 9) {
            string id = newDemoId("ISS-STATE-DEMO");
            registry.createIssue("State lifecycle demo", id,
                                 "Temporary issue used to demonstrate State");
            registry.addChild("feature-auth", id);
            runStateLifecycleDemo(registry, id);
            pauseDemo();
            continue;
        }

        string id = readLine("Issue ID: ");
        Component *issue = registry.getComponent(id);
        if (!issue) {
            cout << "Issue not found.\n";
            pauseDemo();
            continue;
        }

        if (choice == 1) {
            cout << id << " is currently [" << issue->getState() << "]\n";
        } else if (choice == 2) {
            requestState(registry, id, "Assigned", "Assign");
        } else if (choice == 3) {
            requestState(registry, id, "InProgress", "Start work");
        } else if (choice == 4) {
            requestState(registry, id, "Review", "Submit for review");
        } else if (choice == 5) {
            requestState(registry, id, "Closed", "Approve");
        } else if (choice == 6) {
            requestState(registry, id, "InProgress", "Reject review");
        } else if (choice == 7) {
            requestState(registry, id, "Open", "Reopen");
        } else if (choice == 8) {
            string target = readLine(
                "Target exactly as Open/Assigned/InProgress/Review/Closed: ");
            requestState(registry, id, target, "Manual transition request");
        }
        pauseDemo();
    }
}

void iteratorMenu(ProjectRegistry &registry) {
    while (true) {
        title("ITERATOR PATTERN MENU");
        cout << "1. Depth-first traversal of complete hierarchy\n"
             << "2. Unresolved-issue traversal\n"
             << "3. Demonstrate two independent iterators\n"
             << "4. Demonstrate snapshot policy after structural change\n"
             << "0. Back\n";

        int choice = readInt("Choice: ", 0, 4);
        if (choice == 0) return;

        if (choice == 1) {
            string root = readLine("Root ID [repo-taskforge]: ");
            if (root.empty()) root = ROOT_ID;
            registry.printDepthFirst(root);
        } else if (choice == 2) {
            string root = readLine("Root ID [repo-taskforge]: ");
            if (root.empty()) root = ROOT_ID;
            registry.printUnresolved(root);
        } else if (choice == 3) {
            demonstrateIndependentIterators(registry);
        } else if (choice == 4) {
            demonstrateSnapshot(registry);
        }
        pauseDemo();
    }
}

void decoratorMenu(ProjectRegistry &registry) {
    while (true) {
        title("DECORATOR PATTERN MENU");
        cout << "1. Inspect issue description / priority\n"
             << "2. Add PriorityDecorator\n"
             << "3. Add TagDecorator\n"
             << "4. Create a fresh issue and demonstrate stacked decorators\n"
             << "5. Change state through a decorated Component\n"
             << "0. Back\n";

        int choice = readInt("Choice: ", 0, 5);
        if (choice == 0) return;

        if (choice == 4) {
            string id = newDemoId("ISS-DECORATOR-DEMO");
            registry.createIssue("Decorator demo issue", id,
                                 "Base issue before runtime responsibilities");
            registry.addChild("feature-auth", id);
            runDecoratorStackDemo(registry, id);
            pauseDemo();
            continue;
        }

        string id = readLine("Issue ID: ");
        Component *component = registry.getComponent(id);
        if (!component) {
            cout << "Issue not found.\n";
            pauseDemo();
            continue;
        }

        if (choice == 1) {
            printComponentDetails(registry, id);
            cout << "Medium priority: "
                 << (registry.checkPriority(id, "Medium") ? "Yes" : "No")
                 << "\n";
            cout << "Low priority: "
                 << (registry.checkPriority(id, "Low") ? "Yes" : "No")
                 << "\n";
        } else if (choice == 2) {
            string level = readLine("Priority level (High/Medium/Low): ");
            cout << "Before: " << component->getDescription() << "\n";
            registry.addPriority(id, level);
            component = registry.getComponent(id);
            cout << "After:  "
                 << (component ? component->getDescription() : "<missing>")
                 << "\n";
        } else if (choice == 3) {
            string tag = readLine("Tag (e.g. Security, Bug, UI): ");
            cout << "Before: " << component->getDescription() << "\n";
            registry.addTag(id, tag);
            component = registry.getComponent(id);
            cout << "After:  "
                 << (component ? component->getDescription() : "<missing>")
                 << "\n";
        } else if (choice == 5) {
            cout << "The decorator delegates setState() to the wrapped Component.\n";
            string target = readLine(
                "Target state (Open/Assigned/InProgress/Review/Closed): ");
            requestState(registry, id, target,
                         "State request through decorated component");
        }
        pauseDemo();
    }
}

void registryMenu(ProjectRegistry &registry) {
    while (true) {
        title("PROJECT REGISTRY / RUNTIME CHANGE MENU");
        cout << "1. Create Issue\n"
             << "2. Create Project / grouping node\n"
             << "3. Create Repository\n"
             << "4. Attach child to parent\n"
             << "5. Remove child from parent\n"
             << "6. Move child to another parent\n"
             << "7. Check whether an ID exists\n"
             << "8. Inspect component\n"
             << "9. Print tree\n"
             << "0. Back\n";

        int choice = readInt("Choice: ", 0, 9);
        if (choice == 0) return;

        if (choice == 1) {
            string id = readLine("Issue ID: ");
            string name = readLine("Issue name: ");
            string desc = readLine("Description: ");
            registry.createIssue(name, id, desc);
        } else if (choice == 2) {
            string id = readLine("Project ID: ");
            string name = readLine("Project name: ");
            string deadline = readLine("Deadline: ");
            int hours = readInt("Estimated hours (0-10000): ", 0, 10000);
            string desc = readLine("Description: ");
            registry.createProject(name, id, deadline, hours, desc);
        } else if (choice == 3) {
            string id = readLine("Repo ID: ");
            string name = readLine("Repo name: ");
            string desc = readLine("Description: ");
            string owner = readLine("Owner: ");
            bool isPrivate = readYesNo("Private repository?");
            string language = readLine("Language: ");
            registry.createRepo(name, id, desc, owner, isPrivate, language);
        } else if (choice == 4) {
            string parent = readLine("Parent ID: ");
            string child = readLine("Child ID: ");
            registry.addChild(parent, child);
        } else if (choice == 5) {
            string parent = readLine("Parent ID: ");
            string child = readLine("Child ID: ");
            registry.removeChild(parent, child);
        } else if (choice == 6) {
            string child = readLine("Child ID: ");
            string parent = readLine("New parent ID: ");
            registry.moveChild(child, parent);
        } else if (choice == 7) {
            string id = readLine("ID: ");
            cout << (registry.exists(id) ? "Exists." : "Does not exist.") << "\n";
        } else if (choice == 8) {
            printComponentDetails(registry, readLine("Component ID: "));
        } else if (choice == 9) {
            registry.printTree();
        }
        pauseDemo();
    }
}

void printDemoCheatSheet() {
    title("DEMO CHEAT SHEET");
    cout << "If the tutor asks...\n\n"
         << "Where is Composite?\n"
         << "  Component is the common abstraction, Issue is the Leaf, and\n"
         << "  Repo/Project inherit Composite and can contain Components.\n\n"
         << "Where is State?\n"
         << "  Issue is the Context. It stores a State and delegates transition\n"
         << "  validity to concrete State subclasses.\n\n"
         << "Where is Decorator?\n"
         << "  PriorityDecorator and TagDecorator wrap a Component at runtime\n"
         << "  and still expose the Component interface. They can be stacked.\n\n"
         << "Where is Iterator?\n"
         << "  ProjectIterator is the interface. DepthFirstIterator visits the\n"
         << "  whole tree and UnresolvedIssueIterator filters unresolved leaves.\n\n"
         << "What happens if the hierarchy changes during iteration?\n"
         << "  Snapshot policy: an existing iterator keeps its original traversal;\n"
         << "  a new iterator sees the updated hierarchy.\n\n"
         << "Who owns the objects?\n"
         << "  ProjectRegistry owns Components with unique_ptr. Composite children,\n"
         << "  iterators and decorators use non-owning links to the underlying objects.\n";
}

} // namespace

int main() {
    unique_ptr<ProjectRegistry> registry = buildDemoRegistry();

    title("TASKFORGE - INTERACTIVE COS 214 DEMO");
    cout << "Domain: GitHub-style repository, project and issue management\n"
         << "Patterns: Composite | State | Decorator | Iterator\n"
         << "Tip: Choose option 1 for the easiest tutor demonstration.\n";

    while (true) {
        cout << "\n";
        line('=');
        cout << "MAIN MENU\n";
        line('=');
        cout << "1. Guided full-practical demo (recommended for tutor)\n"
             << "2. Composite / hierarchy tests\n"
             << "3. State lifecycle tests\n"
             << "4. Iterator traversal tests\n"
             << "5. Decorator runtime tests\n"
             << "6. Registry / runtime structure operations\n"
             << "7. Print current complete tree\n"
             << "8. Show pattern map\n"
             << "9. Demo cheat sheet\n"
             << "10. Reset all demo data\n"
             << "0. Exit\n";

        int choice = readInt("Choice: ", 0, 10);

        if (choice == 0) {
            title("TASKFORGE DEMO COMPLETE");
            cout << "Exiting cleanly. ProjectRegistry now destroys all owned Components.\n";
            return 0;
        }

        if (choice == 1) {
            guidedDemo(registry);
        } else if (choice == 2) {
            compositeMenu(*registry);
        } else if (choice == 3) {
            stateMenu(*registry);
        } else if (choice == 4) {
            iteratorMenu(*registry);
        } else if (choice == 5) {
            decoratorMenu(*registry);
        } else if (choice == 6) {
            registryMenu(*registry);
        } else if (choice == 7) {
            registry->printTree();
            pauseDemo();
        } else if (choice == 8) {
            printPatternMap();
            pauseDemo();
        } else if (choice == 9) {
            printDemoCheatSheet();
            pauseDemo();
        } else if (choice == 10) {
            registry = buildDemoRegistry();
            cout << "Demo data reset to the original hierarchy and states.\n";
        }
    }
}
