#include "search_users_handler.hpp"

#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

#include <utils/http_response.hpp>

namespace conference_api::handlers {

SearchUsersHandler::SearchUsersHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::PostgresStorage>()) {}

std::string SearchUsersHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    auto& response = request.GetHttpResponse();
    response.SetContentType("application/json");

    const std::string mask = request.GetArg("mask");

    if (mask.empty()) {
        return conference_api::utils::BadRequest(request, "mask is required");
    }

    const auto users = storage_.FindUsersByMask(mask);

    response.SetStatus(userver::server::http::HttpStatus::kOk);

    userver::formats::json::ValueBuilder result(userver::formats::common::Type::kArray);

    for (const auto& user : users) {
        userver::formats::json::ValueBuilder item;
        item["id"] = user.id;
        item["login"] = user.login;
        item["first_name"] = user.first_name;
        item["last_name"] = user.last_name;
        item["role"] = user.role;
        result.PushBack(item.ExtractValue());
    }

    return userver::formats::json::ToString(result.ExtractValue());
}

}