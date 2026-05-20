#pragma once

#include <optional>

#include <models/user.hpp>
#include <storage/postgres_storage.hpp>

#include <userver/server/http/http_request.hpp>

namespace conference_api::auth {

std::optional<conference_api::models::User> GetUserByBearerToken(
    const userver::server::http::HttpRequest& request,
    const conference_api::storage::PostgresStorage& storage
);

bool IsOrganizer(const conference_api::models::User& user);

bool CanCreateTalk(const conference_api::models::User& user);

}