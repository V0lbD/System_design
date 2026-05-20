#pragma once

#include <string>
#include <vector>

namespace conference_api::models {

struct Conference {
    int id{};
    std::string title;
    std::string description;
    std::string date;
    std::vector<int> talk_ids;
};

}