#ifndef ISSUE_H
#define ISSUE_H

#include "Component.h"

class Issue : public Component {

public:
    Issue(const std::string &name,
          const std::string &id,
          const std::string &desc = "");

    ~Issue() override = default;

    // State interface
    std::string getState() const override;
    void setState(const std::string &newState) override;

    // Meaningful GitHub lifecycle actions

    /**
     * @brief Assign the open issue to a developer.
     */
    void assign();

    /**
     * @brief Start working on an assigned issue.
     */
    void startWork();

    /**
     * @brief Submit completed work for review.
     */
    void submitForReview();

    /**
     * @brief Approve an issue currently under review.
     */
    void approve();

    /**
     * @brief Reject an issue under review and return it
     *        to InProgress.
     */
    void reject();

    /**
     * @brief Reopen a previously closed issue.
     */
    void reopen();
};

#endif