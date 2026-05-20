#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component_list.hpp>
#include <userver/components/component.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/congestion_control/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/storages/postgres/component.hpp>

#include <storage/postgres_storage.hpp>

#include <handlers/register_user_handler.hpp>
#include <handlers/login_handler.hpp>
#include <handlers/get_user_by_login_handler.hpp>
#include <handlers/search_users_handler.hpp>
#include <handlers/create_talk_handler.hpp>
#include <handlers/get_all_talks_handler.hpp>
#include <handlers/create_conference_handler.hpp>
#include <handlers/add_talk_to_conference_handler.hpp>
#include <handlers/get_conference_talks_handler.hpp>

int main(int argc, char* argv[]) {
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .AppendComponentList(userver::clients::http::ComponentList())
            .Append<userver::clients::dns::Component>()
            .Append<userver::server::handlers::TestsControl>()
            .Append<userver::congestion_control::Component>()
            .Append<userver::components::Postgres>("postgres-db")
            .Append<conference_api::storage::PostgresStorage>()
            .Append<conference_api::handlers::RegisterUserHandler>()
            .Append<conference_api::handlers::LoginHandler>()
            .Append<conference_api::handlers::GetUserByLoginHandler>()
            .Append<conference_api::handlers::SearchUsersHandler>()
            .Append<conference_api::handlers::CreateTalkHandler>()
            .Append<conference_api::handlers::GetAllTalksHandler>()
            .Append<conference_api::handlers::CreateConferenceHandler>()
            .Append<conference_api::handlers::AddTalkToConferenceHandler>()
            .Append<conference_api::handlers::GetConferenceTalksHandler>();
            

    return userver::utils::DaemonMain(argc, argv, component_list);
}