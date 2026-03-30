#include "get_conference_talks_handler.hpp"

#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

namespace conference_api::handlers {

GetConferenceTalksHandler::GetConferenceTalksHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::InMemoryStorage>()) {}

std::string GetConferenceTalksHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    auto& response = request.GetHttpResponse();
    response.SetContentType("application/json");

    const std::string conference_id_str = request.GetPathArg("conferenceId");
    if (conference_id_str.empty()) {
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);

        userver::formats::json::ValueBuilder error;
        error["message"] = "conferenceId is required";
        return userver::formats::json::ToString(error.ExtractValue());
    }

    const int conference_id = std::stoi(conference_id_str);

    if (!storage_.FindConferenceById(conference_id).has_value()) {
        response.SetStatus(userver::server::http::HttpStatus::kNotFound);

        userver::formats::json::ValueBuilder error;
        error["message"] = "conference not found";
        return userver::formats::json::ToString(error.ExtractValue());
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