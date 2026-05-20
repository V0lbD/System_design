#include "mongo_users_handler.hpp"

#include <chrono>
#include <exception>
#include <string>
#include <unordered_set>

#include <userver/components/component_context.hpp>
#include <userver/formats/bson/inline.hpp>
#include <userver/formats/json.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_method.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/mongo/component.hpp>

namespace conference_api::handlers {

namespace {

bool IsValidRole(const std::string& role) {
    static const std::unordered_set<std::string> kAllowedRoles{
        "participant",
        "speaker",
        "organizer",
    };

    return kAllowedRoles.count(role) > 0;
}

std::string JsonError(
    const userver::server::http::HttpRequest& request,
    userver::server::http::HttpStatus status,
    const std::string& message
) {
    request.GetHttpResponse().SetStatus(status);
    request.GetHttpResponse().SetContentType("application/json");

    userver::formats::json::ValueBuilder result;
    result["error"] = message;
    return userver::formats::json::ToString(result.ExtractValue());
}

}  // namespace

MongoUsersHandler::MongoUsersHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : HttpHandlerBase(config, context),
      mongo_pool_(
          context.FindComponent<userver::components::Mongo>("mongo-db").GetPool()
      ) {}

std::string MongoUsersHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    request.GetHttpResponse().SetContentType("application/json");

    if (request.GetMethod() == userver::server::http::HttpMethod::kGet) {
        return HandleGet(request);
    }

    if (request.GetMethod() == userver::server::http::HttpMethod::kPost) {
        return HandlePost(request);
    }

    return JsonError(
        request,
        userver::server::http::HttpStatus::kMethodNotAllowed,
        "method not allowed"
    );
}

std::string MongoUsersHandler::HandleGet(
    const userver::server::http::HttpRequest& request
) const {
    using userver::formats::bson::MakeDoc;

    auto users = mongo_pool_->GetCollection("users");

    const auto total_count = users.Count(MakeDoc());
    const auto speakers_count = users.Count(MakeDoc("role", "speaker"));
    const auto organizers_count = users.Count(MakeDoc("role", "organizer"));
    const auto participants_count = users.Count(MakeDoc("role", "participant"));

    request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kOk);

    userver::formats::json::ValueBuilder result;
    result["storage"] = "mongodb";
    result["collection"] = "users";
    result["total_count"] = static_cast<int>(total_count);
    result["speakers_count"] = static_cast<int>(speakers_count);
    result["organizers_count"] = static_cast<int>(organizers_count);
    result["participants_count"] = static_cast<int>(participants_count);

    return userver::formats::json::ToString(result.ExtractValue());
}

std::string MongoUsersHandler::HandlePost(
    const userver::server::http::HttpRequest& request
) const {
    using userver::formats::bson::MakeDoc;

    userver::formats::json::Value json;

    try {
        json = userver::formats::json::FromString(request.RequestBody());
    } catch (const std::exception&) {
        return JsonError(
            request,
            userver::server::http::HttpStatus::kBadRequest,
            "invalid json"
        );
    }

    const auto login = json["login"].As<std::string>("");
    const auto password = json["password"].As<std::string>("");
    const auto first_name = json["first_name"].As<std::string>("");
    const auto last_name = json["last_name"].As<std::string>("");
    const auto role = json["role"].As<std::string>("participant");
    const auto email = json["email"].As<std::string>("");

    if (
        login.empty() ||
        password.empty() ||
        first_name.empty() ||
        last_name.empty() ||
        email.empty()
    ) {
        return JsonError(
            request,
            userver::server::http::HttpStatus::kBadRequest,
            "login, password, first_name, last_name and email are required"
        );
    }

    if (!IsValidRole(role)) {
        return JsonError(
            request,
            userver::server::http::HttpStatus::kBadRequest,
            "role must be one of: participant, speaker, organizer"
        );
    }

    auto users = mongo_pool_->GetCollection("users");
    const auto now = std::chrono::system_clock::now();

    try {
        users.InsertOne(MakeDoc(
            "login", login,
            "password_hash", "hash_" + password,
            "first_name", first_name,
            "last_name", last_name,
            "role", role,
            "email", email,
            "bio", "Created through C++ userver MongoDB API",
            "rating", 0,
            "created_at", now,
            "updated_at", now
        ));
    } catch (const std::exception& error) {
        return JsonError(
            request,
            userver::server::http::HttpStatus::kConflict,
            std::string("failed to create MongoDB user: ") + error.what()
        );
    }

    request.GetHttpResponse().SetStatus(userver::server::http::HttpStatus::kCreated);

    userver::formats::json::ValueBuilder result;
    result["storage"] = "mongodb";
    result["login"] = login;
    result["first_name"] = first_name;
    result["last_name"] = last_name;
    result["role"] = role;
    result["email"] = email;

    return userver::formats::json::ToString(result.ExtractValue());
}

}  // namespace conference_api::handlers
