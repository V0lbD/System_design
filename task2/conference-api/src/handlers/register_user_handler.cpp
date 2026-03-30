#include "register_user_handler.hpp"

#include <stdexcept>
#include <string>

#include <userver/formats/json.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

namespace conference_api::handlers {

RegisterUserHandler::RegisterUserHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::InMemoryStorage>()) {}

std::string RegisterUserHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    const auto json = userver::formats::json::FromString(request.RequestBody());

    const std::string login = json["login"].As<std::string>("");
    const std::string password = json["password"].As<std::string>("");
    const std::string first_name = json["first_name"].As<std::string>("");
    const std::string last_name = json["last_name"].As<std::string>("");
    const std::string role = json["role"].As<std::string>("attendee");

    if (login.empty() || password.empty() || first_name.empty() || last_name.empty()) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
        response.SetContentType("application/json");

        userver::formats::json::ValueBuilder error;
        error["message"] = "login, password, first_name and last_name are required";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    try {
        const auto& user = storage_.CreateUser(login, password, first_name, last_name, role);

        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kCreated);
        response.SetContentType("application/json");

        userver::formats::json::ValueBuilder result;
        result["id"] = user.id;
        result["login"] = user.login;
        result["first_name"] = user.first_name;
        result["last_name"] = user.last_name;
        result["role"] = user.role;

        return userver::formats::json::ToString(result.ExtractValue());
    } catch (const std::runtime_error&) {
        auto& response = request.GetHttpResponse();
        response.SetStatus(userver::server::http::HttpStatus::kConflict);
        response.SetContentType("application/json");

        userver::formats::json::ValueBuilder error;
        error["message"] = "user with this login already exists";
        
        return userver::formats::json::ToString(error.ExtractValue());
    }
}

}