#pragma once
#include <string>

class LoreEntry {
public:

    std::string content;

    LoreEntry(const std::string& c) : content(c) {
    }
};