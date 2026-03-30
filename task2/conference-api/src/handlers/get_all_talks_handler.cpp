#include "get_all_talks_handler.hpp"

#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_status.hpp>

namespace conference_api::handlers {

GetAllTalksHandler::GetAllTalksHandler(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      storage_(context.FindComponent<conference_api::storage::InMemoryStorage>()) {}

std::string GetAllTalksHandler::HandleRequestThrow(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& /*context*/
) const {
    (void)request;

    auto& response = request.GetHttpResponse();
    response.SetContentType("application/json");
    response.SetStatus(userver::server::http::HttpStatus::kOk);

    const auto talks = storage_.GetAllTalks();

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