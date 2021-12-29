#include "helper.h"

uint8_t get_first_index_of_one(uint16_t value) {
    uint8_t index = 0;
    for(index = 0; index < 16; index++) {
        if(value & (1 << index)) break;
    }
    return index;
}

void replace_all(std::string& source, const std::string& from, const std::string& to) {
    std::string newString;
    newString.reserve(source.length()); // avoids a few memory allocations

    std::string::size_type lastPos = 0;
    std::string::size_type findPos;

    while(std::string::npos != (findPos = source.find(from, lastPos))) {
        newString.append(source, lastPos, findPos - lastPos);
        newString += to;
        lastPos = findPos + from.length();
    }

    // Care for the rest after last occurrence
    newString += source.substr(lastPos);

    source.swap(newString);
}

void normalize_path(std::string& path) {
    replace_all(path, "\\", "/");
    while(path.find("//") != std::string::npos) {
        replace_all(path, "//", "/");
    }
    if(path[0] == '.') path = path.substr(1, path.length() - 1);
    if(path[0] == '/') path = path.substr(1, path.length() - 1);
}