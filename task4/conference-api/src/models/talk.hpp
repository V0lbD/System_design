#pragma once

#include <string>

namespace conference_api::models {

struct Talk {
    int id{};
    std::string title;
    std::string description;
    int speaker_id{};
};

}