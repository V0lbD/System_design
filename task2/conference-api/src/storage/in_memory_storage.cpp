#include "in_memory_storage.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace conference_api::storage {
namespace {

std::string ToLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) {
                       return static_cast<char>(std::tolower(c));
                   });
    return value;
}

bool ContainsCaseInsensitive(const std::string& text, const std::string& mask) {
    return ToLower(text).find(ToLower(mask)) != std::string::npos;
}

}

// in-memory достаточно для выполнения лабораторной работы
InMemoryStorage::InMemoryStorage(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : userver::components::ComponentBase(config, context) {}

const models::User& InMemoryStorage::CreateUser(const std::string& login,
                                                const std::string& password,
                                                const std::string& first_name,
                                                const std::string& last_name,
                                                const std::string& role) {
    if (FindUserByLogin(login).has_value()) {
        throw std::runtime_error("User with this login already exists");
    }

    models::User user;
    user.id = next_user_id_++;
    user.login = login;
    user.password = password;
    user.first_name = first_name;
    user.last_name = last_name;
    user.role = role;

    auto [it, inserted] = users_.emplace(user.id, user);
    (void)inserted;
    return it->second;
}

std::optional<models::User> InMemoryStorage::FindUserByLogin(
    const std::string& login) const {
    for (const auto& [id, user] : users_) {
        (void)id;
        if (user.login == login) {
            return user;
        }
    }
    return std::nullopt;
}

std::vector<models::User> InMemoryStorage::FindUsersByMask(
    const std::string& mask) const {
    std::vector<models::User> result;

    for (const auto& [id, user] : users_) {
        (void)id;
        const std::string full_name = user.first_name + " " + user.last_name;
        if (ContainsCaseInsensitive(user.first_name, mask) ||
            ContainsCaseInsensitive(user.last_name, mask) ||
            ContainsCaseInsensitive(full_name, mask)) {
            result.push_back(user);
        }
    }

    return result;
}

std::optional<models::User> InMemoryStorage::ValidateUser(
    const std::string& login,
    const std::string& password
) const {
    const auto user = FindUserByLogin(login);
    if (!user.has_value()) {
        return std::nullopt;
    }

    if (user->password != password) {
        return std::nullopt;
    }

    return user;
}

std::string InMemoryStorage::CreateToken(int user_id) {
    const std::string token = "token_" + std::to_string(next_token_id_++);
    tokens_[token] = user_id;
    return token;
}

std::optional<models::User> InMemoryStorage::FindUserByToken(
    const std::string& token
) const {
    const auto it = tokens_.find(token);
    if (it == tokens_.end()) {
        return std::nullopt;
    }

    const int user_id = it->second;
    const auto user_it = users_.find(user_id);
    if (user_it == users_.end()) {
        return std::nullopt;
    }

    return user_it->second;
}

const models::Talk& InMemoryStorage::CreateTalk(const std::string& title,
                                                const std::string& description,
                                                int speaker_id) {
    models::Talk talk;
    talk.id = next_talk_id_++;
    talk.title = title;
    talk.description = description;
    talk.speaker_id = speaker_id;

    auto [it, inserted] = talks_.emplace(talk.id, talk);
    (void)inserted;
    return it->second;
}

std::vector<models::Talk> InMemoryStorage::GetAllTalks() const {
    std::vector<models::Talk> result;
    result.reserve(talks_.size());

    for (const auto& [id, talk] : talks_) {
        (void)id;
        result.push_back(talk);
    }

    return result;
}

const models::Conference& InMemoryStorage::CreateConference(
    const std::string& title,
    const std::string& description,
    const std::string& date) {
    models::Conference conference;
    conference.id = next_conference_id_++;
    conference.title = title;
    conference.description = description;
    conference.date = date;

    auto [it, inserted] = conferences_.emplace(conference.id, conference);
    (void)inserted;
    return it->second;
}

std::optional<models::Conference> InMemoryStorage::FindConferenceById(int conference_id) const {
    const auto it = conferences_.find(conference_id);
    if (it == conferences_.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<models::Talk> InMemoryStorage::FindTalkById(int talk_id) const {
    const auto it = talks_.find(talk_id);
    if (it == talks_.end()) {
        return std::nullopt;
    }
    return it->second;
}

bool InMemoryStorage::AddTalkToConference(int conference_id, int talk_id) {
    auto conf_it = conferences_.find(conference_id);
    if (conf_it == conferences_.end()) {
        return false;
    }

    auto talk_it = talks_.find(talk_id);
    if (talk_it == talks_.end()) {
        return false;
    }

    auto& talk_ids = conf_it->second.talk_ids;
    if (std::find(talk_ids.begin(), talk_ids.end(), talk_id) != talk_ids.end()) {
        return false;
    }

    talk_ids.push_back(talk_id);
    return true;
}

std::vector<models::Talk> InMemoryStorage::GetConferenceTalks(int conference_id) const {
    std::vector<models::Talk> result;

    const auto conf_it = conferences_.find(conference_id);
    if (conf_it == conferences_.end()) {
        return result;
    }

    for (int talk_id : conf_it->second.talk_ids) {
        const auto talk_it = talks_.find(talk_id);
        if (talk_it != talks_.end()) {
            result.push_back(talk_it->second);
        }
    }

    return result;
}

userver::yaml_config::Schema InMemoryStorage::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<userver::components::ComponentBase>(R"(
        type: object
        description: in-memory storage component
        additionalProperties: false
        properties: {}
)");
}

}