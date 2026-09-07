
#include "Repo.h"
#include "Composite.h"

Repo::Repo(const std::string &name, const std::string &id,
           const std::string &desc, std::string owner, bool privateRepo,
           std::string language)
    : Composite(name, id, desc), owner(owner), privateRepo(privateRepo),
      language(language) {}

std::string Repo::getDescription() const {
    return "[" + owner + "] "+ description + "(" + language + ")";
}
