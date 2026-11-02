#pragma once
#include <string>

class Output {
public:
    static void error(std::string msg);
    static void print(std::string msg);
};