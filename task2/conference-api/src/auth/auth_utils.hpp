#pragma once

#include <optional>
#include <string>

#include <userver/server/http/http_request.hpp>

#include <models/user.hpp>
#include <storage/in_memory_storage.hpp>

namespace conference_api::auth {

std::optional<conference_api::models::User> GetUserByBearerToken(
    const userver::server::http::HttpRequest& request,
    const conference_api::storage::InMemoryStorage& storage
);

bool IsOrganizer(const conference_api::models::User& user);
bool CanCreateTalk(const conference_api::models::User& user);

}  // namespace conference_api::auth