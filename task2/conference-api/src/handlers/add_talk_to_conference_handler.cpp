#include "add_talk_to_conference_handler.hpp"

#include <exception>

#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

#include <auth/auth_utils.hpp>
#include <utils/http_response.hpp>

namespace conference_api::handlers {

AddTalkToConferenceHandler::AddTalkToConferenceHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::InMemoryStorage>()) {}

std::string AddTalkToConferenceHandler::HandleRequestThrow(
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

    const std::string conference_id_str = request.GetPathArg("conferenceId");
    const std::string talk_id_str = request.GetPathArg("talkId");

    if (conference_id_str.empty() || talk_id_str.empty()) {
        return conference_api::utils::BadRequest(request, "conferenceId and talkId are required");
    }

    int conference_id = 0;
    int talk_id = 0;
    try {
        conference_id = std::stoi(conference_id_str);
        talk_id = std::stoi(talk_id_str);
    } catch (const std::exception&) {
        return conference_api::utils::BadRequest(request, "conferenceId and talkId must be integers");
    }

    if (!storage_.FindConferenceById(conference_id).has_value()) {
        return conference_api::utils::NotFound(request, "conference not found");
    }

    if (!storage_.FindTalkById(talk_id).has_value()) {
        return conference_api::utils::NotFound(request, "talk not found");
    }

    const bool added = storage_.AddTalkToConference(conference_id, talk_id);
    if (!added) {
        return conference_api::utils::Conflict(request, "talk already added to conference");
    }

    response.SetStatus(userver::server::http::HttpStatus::kCreated);

    userver::formats::json::ValueBuilder result;
    result["message"] = "talk added to conference";

    return userver::formats::json::ToString(result.ExtractValue());
}

}