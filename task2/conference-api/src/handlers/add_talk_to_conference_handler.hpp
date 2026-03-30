#pragma once

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include <storage/in_memory_storage.hpp>

namespace conference_api::handlers {

class AddTalkToConferenceHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-add-talk-to-conference";

    AddTalkToConferenceHandler(const userver::components::ComponentConfig& config,
                               const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const override;

private:
    conference_api::storage::InMemoryStorage& storage_;
};

}  // namespace conference_api::handlers