#include "login_handler.hpp"

#include <userver/formats/json.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

namespace conference_api::handlers {

LoginHandler::LoginHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::InMemoryStorage>()) {}

std::string LoginHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    const auto json = userver::formats::json::FromString(request.RequestBody());

    const std::string login = json["login"].As<std::string>("");
    const std::string password = json["password"].As<std::string>("");

    auto& response = request.GetHttpResponse();
    response.SetContentType("application/json");

    const auto user = storage_.ValidateUser(login, password);
    if (!user.has_value()) {
        response.SetStatus(userver::server::http::HttpStatus::kUnauthorized);

        userver::formats::json::ValueBuilder error;
        error["message"] = "invalid credentials";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    const auto token = storage_.CreateToken(user->id);

    response.SetStatus(userver::server::http::HttpStatus::kOk);

    userver::formats::json::ValueBuilder result;
    result["token"] = token;

    return userver::formats::json::ToString(result.ExtractValue());
}

}