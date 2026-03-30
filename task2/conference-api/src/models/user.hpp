#pragma once

#include <string>

namespace conference_api::models {

struct User {
    int id{};
    std::string login;
    std::string password;
    std::string first_name;
    std::string last_name;
    std::string role;
};

}