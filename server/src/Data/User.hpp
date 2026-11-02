#pragma once
#include <cstddef>
#include <string>
#include <vector>

// Dummy structs.

struct UserData {

    struct UserConnect {
        std::string username;
        std::string password;
    };

    struct Characters {
        std::string name;
        int level;
    };

    struct User {
        std::string username;
        std::vector<Characters> characters;
    };
};