#include "auth_utils.hpp"

namespace conference_api::auth {

std::optional<conference_api::models::User> GetUserByBearerToken(
    const userver::server::http::HttpRequest& request,
    const conference_api::storage::PostgresStorage& storage
) {
    const std::string auth_header = request.GetHeader("Authorization");
    const std::string prefix = "Bearer ";

    if (auth_header.size() <= prefix.size()) {
        return std::nullopt;
    }

    if (auth_header.compare(0, prefix.size(), prefix) != 0) {
        return std::nullopt;
    }

    const std::string token = auth_header.substr(prefix.size());
    if (token.empty()) {
        return std::nullopt;
    }

    return storage.FindUserByToken(token);
}

bool IsOrganizer(const conference_api::models::User& user) {
    return user.role == "organizer";
}

bool CanCreateTalk(const conference_api::models::User& user) {
    return user.role == "speaker" || user.role == "organizer";
}

}