#pragma once

#include <string>
#include <string_view>

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/mongo/pool.hpp>

namespace conference_api::handlers {

class MongoUsersHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-mongo-users";

    MongoUsersHandler(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );

    std::string HandleRequestThrow(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& context
    ) const override;

private:
    std::string HandleGet(
        const userver::server::http::HttpRequest& request
    ) const;

    std::string HandlePost(
        const userver::server::http::HttpRequest& request
    ) const;

    userver::storages::mongo::PoolPtr mongo_pool_;
};

}  // namespace conference_api::handlers
