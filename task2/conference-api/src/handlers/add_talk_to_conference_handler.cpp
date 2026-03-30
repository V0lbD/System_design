#include "add_talk_to_conference_handler.hpp"

#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

#include <auth/auth_utils.hpp>

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
        response.SetStatus(userver::server::http::HttpStatus::kUnauthorized);

        userver::formats::json::ValueBuilder error;
        error["message"] = "authorization required";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    const std::string conference_id_str = request.GetPathArg("conferenceId");
    const std::string talk_id_str = request.GetPathArg("talkId");

    if (conference_id_str.empty() || talk_id_str.empty()) {
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);

        userver::formats::json::ValueBuilder error;
        error["message"] = "conferenceId and talkId are required";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    const int conference_id = std::stoi(conference_id_str);
    const int talk_id = std::stoi(talk_id_str);

    if (!storage_.FindConferenceById(conference_id).has_value()) {
        response.SetStatus(userver::server::http::HttpStatus::kNotFound);

        userver::formats::json::ValueBuilder error;
        error["message"] = "conference not found";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    if (!storage_.FindTalkById(talk_id).has_value()) {
        response.SetStatus(userver::server::http::HttpStatus::kNotFound);

        userver::formats::json::ValueBuilder error;
        error["message"] = "talk not found";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    const bool added = storage_.AddTalkToConference(conference_id, talk_id);
    if (!added) {
        response.SetStatus(userver::server::http::HttpStatus::kConflict);

        userver::formats::json::ValueBuilder error;
        error["message"] = "talk already added to conference";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    response.SetStatus(userver::server::http::HttpStatus::kCreated);

    userver::formats::json::ValueBuilder result;
    result["message"] = "talk added to conference";

    return userver::formats::json::ToString(result.ExtractValue());
}

}  // namespace conference_api::handlers