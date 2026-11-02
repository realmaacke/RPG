#include "Output.hpp"
#include <iostream>

void Output::error(std::string msg) {
    std::cerr << msg << std::endl;
}

void Output::print(std::string msg) {
    std::cout << msg << std::endl;
}