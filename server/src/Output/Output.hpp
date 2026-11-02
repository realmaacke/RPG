#pragma once

#include <iostream>
class Output {
public:
    static void error(const std::string& msg) {
        std::cerr << msg << std::endl;
    }

    static void print(const std::string& msg) {
        std::cout << msg << std::endl;
    }
};