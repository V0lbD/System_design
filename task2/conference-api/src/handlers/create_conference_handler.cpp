#include "create_conference_handler.hpp"

#include <userver/formats/json.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

#include <auth/auth_utils.hpp>

namespace conference_api::handlers {

CreateConferenceHandler::CreateConferenceHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::InMemoryStorage>()) {}

std::string CreateConferenceHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    auto& response = request.GetHttpResponse();
    response.SetContentType("application/json");

    const auto user = conference_api::auth::GetUserByBearerToken(request, storage_);
    if (!user.has_value()) {
        response.SetStatus(userver::server::http::HttpStatus::kUnauthorized);

        userver::formats::json::ValueBuilder error;
        error["message"] = "authorization required";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    const auto json = userver::formats::json::FromString(request.RequestBody());

    const std::string title = json["title"].As<std::string>("");
    const std::string description = json["description"].As<std::string>("");
    const std::string date = json["date"].As<std::string>("");

    if (title.empty() || description.empty() || date.empty()) {
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);

        userver::formats::json::ValueBuilder error;
        error["message"] = "title, description and date are required";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    const auto& conference = storage_.CreateConference(title, description, date);

    response.SetStatus(userver::server::http::HttpStatus::kCreated);

    userver::formats::json::ValueBuilder result;
    result["id"] = conference.id;
    result["title"] = conference.title;
    result["description"] = conference.description;
    result["date"] = conference.date;

    return userver::formats::json::ToString(result.ExtractValue());
}

}