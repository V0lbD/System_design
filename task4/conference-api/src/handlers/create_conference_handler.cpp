#include "create_conference_handler.hpp"

#include <exception>

#include <userver/formats/json.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

#include <auth/auth_utils.hpp>
#include <utils/http_response.hpp>

namespace conference_api::handlers {

CreateConferenceHandler::CreateConferenceHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::PostgresStorage>()) {}

std::string CreateConferenceHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    auto& response = request.GetHttpResponse();
    response.SetContentType("application/json");

    const auto user = conference_api::auth::GetUserByBearerToken(request, storage_);
    if (!user.has_value()) {
        return conference_api::utils::Unauthorized(request, "authorization required");
    }

    if (!conference_api::auth::IsOrganizer(*user)) {
        return conference_api::utils::Forbidden(request, "organizer role required");
    }

    userver::formats::json::Value json;
    try {
        json = userver::formats::json::FromString(request.RequestBody());
    } catch (const std::exception&) {
        return conference_api::utils::BadRequest(request, "invalid json");
    }

    const std::string title = json["title"].As<std::string>("");
    const std::string description = json["description"].As<std::string>("");
    const std::string date = json["date"].As<std::string>("");

    if (title.empty() || description.empty() || date.empty()) {
        return conference_api::utils::BadRequest(request, "title, description and date are required");
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