#pragma once
#include <string>

struct Todo {
    int id = 0;
    std::string text;
    bool isChecked = false;
};
