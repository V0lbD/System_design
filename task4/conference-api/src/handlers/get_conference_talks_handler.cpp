#include "get_conference_talks_handler.hpp"

#include <exception>

#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

#include <utils/http_response.hpp>

namespace conference_api::handlers {

GetConferenceTalksHandler::GetConferenceTalksHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::PostgresStorage>()) {}

std::string GetConferenceTalksHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    auto& response = request.GetHttpResponse();
    response.SetContentType("application/json");

    const std::string conference_id_str = request.GetPathArg("conferenceId");
    if (conference_id_str.empty()) {
        return conference_api::utils::BadRequest(request, "conferenceId is required");
    }

    int conference_id = 0;
    try {
        conference_id = std::stoi(conference_id_str);
    } catch (const std::exception&) {
        return conference_api::utils::BadRequest(request, "conferenceId must be an integer");
    }

    if (!storage_.FindConferenceById(conference_id).has_value()) {
        return conference_api::utils::NotFound(request, "conference not found");
    }

    const auto talks = storage_.GetConferenceTalks(conference_id);

    response.SetStatus(userver::server::http::HttpStatus::kOk);

    userver::formats::json::ValueBuilder result(userver::formats::common::Type::kArray);

    for (const auto& talk : talks) {
        userver::formats::json::ValueBuilder item;
        item["id"] = talk.id;
        item["title"] = talk.title;
        item["description"] = talk.description;
        item["speaker_id"] = talk.speaker_id;
        result.PushBack(item.ExtractValue());
    }

    return userver::formats::json::ToString(result.ExtractValue());
}

}