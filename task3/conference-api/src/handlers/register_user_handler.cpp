#include "register_user_handler.hpp"

#include <stdexcept>
#include <string>
#include <unordered_set>

#include <userver/formats/json.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>
#include <utils/http_response.hpp>

namespace conference_api::handlers {
namespace {

bool IsValidRole(const std::string& role) {
    static const std::unordered_set<std::string> kAllowedRoles{
        "attendee",
        "speaker",
        "organizer",
    };
    return kAllowedRoles.count(role) > 0;
}

}

RegisterUserHandler::RegisterUserHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::PostgresStorage>()) {}

std::string RegisterUserHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    auto& response = request.GetHttpResponse();
    response.SetContentType("application/json");

    userver::formats::json::Value json;
    try {
        json = userver::formats::json::FromString(request.RequestBody());
    } catch (const std::exception&) {
        return conference_api::utils::BadRequest(request, "invalid json");
    }

    const std::string login = json["login"].As<std::string>("");
    const std::string password = json["password"].As<std::string>("");
    const std::string first_name = json["first_name"].As<std::string>("");
    const std::string last_name = json["last_name"].As<std::string>("");
    const std::string role = json["role"].As<std::string>("attendee");

    if (login.empty() || password.empty() || first_name.empty() || last_name.empty()) {
        return conference_api::utils::BadRequest(request, "login, password, first_name and last_name are required");
    }

    if (!IsValidRole(role)) {
        return conference_api::utils::BadRequest(request, "role must be one of: attendee, speaker, organizer");
    }

    try {
        const auto& user = storage_.CreateUser(login, password, first_name, last_name, role);

        response.SetStatus(userver::server::http::HttpStatus::kCreated);

        userver::formats::json::ValueBuilder result;
        result["id"] = user.id;
        result["login"] = user.login;
        result["first_name"] = user.first_name;
        result["last_name"] = user.last_name;
        result["role"] = user.role;

        return userver::formats::json::ToString(result.ExtractValue());
    } catch (const std::runtime_error&) {
        return conference_api::utils::Conflict(request, "user with this login already exists");
    }
}

}