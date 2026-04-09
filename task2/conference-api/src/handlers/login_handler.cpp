#include "login_handler.hpp"

#include <exception>

#include <userver/formats/json.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>
#include <utils/http_response.hpp>

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

    const auto user = storage_.ValidateUser(login, password);
    if (!user.has_value()) {
        return conference_api::utils::Unauthorized(request, "invalid credentials");
    }

    const auto token = storage_.CreateToken(user->id);

    response.SetStatus(userver::server::http::HttpStatus::kOk);

    userver::formats::json::ValueBuilder result;
    result["token"] = token;

    return userver::formats::json::ToString(result.ExtractValue());
}

}