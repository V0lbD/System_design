#pragma once

#include <optional>
#include <string>
#include <vector>

#include <userver/components/component_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/yaml_config/schema.hpp>

#include <models/user.hpp>
#include <models/talk.hpp>
#include <models/conference.hpp>

namespace conference_api::storage {

class PostgresStorage final : public userver::components::ComponentBase {
public:
    static constexpr std::string_view kName = "postgres-storage";

    PostgresStorage(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context);


    // user
    const models::User CreateUser(
        const std::string& login,
        const std::string& password,
        const std::string& first_name,
        const std::string& last_name,
        const std::string& role
    ) const;

    std::optional<models::User> FindUserByLogin(const std::string& login) const;

    std::vector<models::User> FindUsersByMask(const std::string& mask) const;

    std::optional<models::User> ValidateUser(
        const std::string& login,
        const std::string& password
    ) const;

    // token
    std::string CreateToken(std::int64_t user_id) const;

    std::optional<models::User> FindUserByToken(const std::string& token) const;

    static userver::yaml_config::Schema GetStaticConfigSchema();

    // talk
    const models::Talk CreateTalk(
        const std::string& title,
        const std::string& description,
        int speaker_id
    ) const;

    std::vector<models::Talk> GetAllTalks() const;

    std::optional<models::Talk> FindTalkById(int talk_id) const;

    // conference
    const models::Conference CreateConference(
        const std::string& title,
        const std::string& description,
        const std::string& date
    ) const;

    std::optional<models::Conference> FindConferenceById(int conference_id) const;

    bool AddTalkToConference(int conference_id, int talk_id) const;

    std::vector<models::Talk> GetConferenceTalks(int conference_id) const;

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}