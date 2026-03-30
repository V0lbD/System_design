#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <userver/components/component_base.hpp>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>

#include <models/conference.hpp>
#include <models/talk.hpp>
#include <models/user.hpp>

namespace conference_api::storage {

class InMemoryStorage final : public userver::components::ComponentBase {
public:
    static constexpr std::string_view kName = "in-memory-storage";

    InMemoryStorage(const userver::components::ComponentConfig& config,
                    const userver::components::ComponentContext& context);

    const models::User& CreateUser(const std::string& login,
                                   const std::string& password,
                                   const std::string& first_name,
                                   const std::string& last_name,
                                   const std::string& role);

    std::optional<models::User> FindUserByLogin(const std::string& login) const;
    std::vector<models::User> FindUsersByMask(const std::string& mask) const;

    static userver::yaml_config::Schema GetStaticConfigSchema();

    std::optional<models::User> ValidateUser(
        const std::string& login,
        const std::string& password
    ) const;

    std::string CreateToken(int user_id);

    std::optional<models::User> FindUserByToken(
        const std::string& token
    ) const;

    const models::Talk& CreateTalk(const std::string& title,
                               const std::string& description,
                               int speaker_id);

    std::vector<models::Talk> GetAllTalks() const;

    const models::Conference& CreateConference(const std::string& title,
                                           const std::string& description,
                                           const std::string& date);

    bool AddTalkToConference(int conference_id, int talk_id);

    std::optional<models::Conference> FindConferenceById(int conference_id) const;
    std::optional<models::Talk> FindTalkById(int talk_id) const;
    std::vector<models::Talk> GetConferenceTalks(int conference_id) const;

private:
    std::unordered_map<int, models::User> users_;
    int next_user_id_{1};
    std::unordered_map<std::string, int> tokens_;
    int next_token_id_{1};
    std::unordered_map<int, models::Talk> talks_;
    int next_talk_id_{1};
    std::unordered_map<int, models::Conference> conferences_;
    int next_conference_id_{1};
};

}