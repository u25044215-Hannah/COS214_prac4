
#include "Repo.h"
#include "Composite.h"

using namespace std;

Repo::Repo(const string &name, const string &id, const string &desc,
           string owner, bool privateRepo, string language)
    : Composite(name, id, desc), owner(owner), privateRepo(privateRepo),
      language(language) {}

string Repo::getDescription() const {
  string repoStatus = privateRepo ? "Private" : "Public";
  return "[" + repoStatus + "] [" + owner + "] " + description + " (" + language +
         ")";
}
