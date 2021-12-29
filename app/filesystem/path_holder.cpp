#include <set>
#include <string>
#include "path_holder.h"

namespace PathHolder {

struct FileComparator {
    bool operator()(std::string* a, std::string* b) const {
        return *a < *b;
    }
};

static std::set<std::string*, FileComparator> files_set;

bool contain(std::string* file) {
    bool result = false;

    if(files_set.find(file) != files_set.end()) {
        result = true;
    }

    return result;
}

void insert(std::string* file) {
    files_set.insert(file);
}

void erase(std::string* file) {
    auto itr = files_set.find(file);
    if(itr != files_set.end()) {
        files_set.erase(itr);
    }
}

}
