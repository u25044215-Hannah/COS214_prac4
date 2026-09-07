#ifndef REPO_H
#define REPO_H
#include "Composite.h"

class Repo : public Composite {
private:
  std::string owner;
  bool privateRepo;
  std::string language;

public:
  Repo(const std::string &name, const std::string &id, const std::string &desc,
       std::string owner, bool privateRepo, std::string language);
  virtual ~Repo() = default;
  virtual std::string getDescription() const override;
};

#endif