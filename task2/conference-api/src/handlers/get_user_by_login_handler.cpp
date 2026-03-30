#include "get_user_by_login_handler.hpp"

#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

namespace conference_api::handlers {

GetUserByLoginHandler::GetUserByLoginHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::InMemoryStorage>()) {}

std::string GetUserByLoginHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    auto& response = request.GetHttpResponse();
    response.SetContentType("application/json");

    const std::string login = request.GetArg("login");

    if (login.empty()) {
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);

        userver::formats::json::ValueBuilder error;
        error["message"] = "query parameter 'login' is required";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    const auto user = storage_.FindUserByLogin(login);
    if (!user.has_value()) {
        response.SetStatus(userver::server::http::HttpStatus::kNotFound);

        userver::formats::json::ValueBuilder error;
        error["message"] = "user not found";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    response.SetStatus(userver::server::http::HttpStatus::kOk);

    userver::formats::json::ValueBuilder result;
    result["id"] = user->id;
    result["login"] = user->login;
    result["first_name"] = user->first_name;
    result["last_name"] = user->last_name;
    result["role"] = user->role;

    return userver::formats::json::ToString(result.ExtractValue());
}

}