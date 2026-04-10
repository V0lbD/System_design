#include "postgres_storage.hpp"

#include <random>
#include <stdexcept>
#include <string>

#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/io/enum_types.hpp>
#include <userver/storages/postgres/io/row_types.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/result_set.hpp>
#include <userver/storages/postgres/row.hpp>

namespace conference_api::storage {
namespace {

std::string GenerateToken() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(100000, 999999);
    return "token_" + std::to_string(dist(gen));
}

const userver::storages::postgres::Query kInsertUser{
    "INSERT INTO users(login, password, first_name, last_name, role) "
    "VALUES($1, $2, $3, $4, $5) "
    "RETURNING id, login, password, first_name, last_name, role",
    userver::storages::postgres::Query::Name{"insert_user"},
};

const userver::storages::postgres::Query kSelectUserByLogin{
    "SELECT id, login, password, first_name, last_name, role "
    "FROM users WHERE login = $1",
    userver::storages::postgres::Query::Name{"select_user_by_login"},
};

const userver::storages::postgres::Query kSearchUsersByMask{
    "SELECT id, login, password, first_name, last_name, role "
    "FROM users "
    "WHERE LOWER(first_name) LIKE LOWER($1) "
    "   OR LOWER(last_name) LIKE LOWER($1) "
    "   OR LOWER(first_name || ' ' || last_name) LIKE LOWER($1) "
    "ORDER BY id",
    userver::storages::postgres::Query::Name{"search_users_by_mask"},
};

const userver::storages::postgres::Query kInsertToken{
    "INSERT INTO tokens(token, user_id) VALUES($1, $2)",
    userver::storages::postgres::Query::Name{"insert_token"},
};

const userver::storages::postgres::Query kSelectUserByToken{
    "SELECT u.id, u.login, u.password, u.first_name, u.last_name, u.role "
    "FROM tokens t "
    "JOIN users u ON u.id = t.user_id "
    "WHERE t.token = $1",
    userver::storages::postgres::Query::Name{"select_user_by_token"},
};

models::User ReadUserFromRow(const userver::storages::postgres::Row& row) {
    models::User user;
    user.id = static_cast<int>(row["id"].As<std::int64_t>());
    user.login = row["login"].As<std::string>();
    user.password = row["password"].As<std::string>();
    user.first_name = row["first_name"].As<std::string>();
    user.last_name = row["last_name"].As<std::string>();
    user.role = row["role"].As<std::string>();
    return user;
}

models::Talk ReadTalkFromRow(const userver::storages::postgres::Row& row) {
    models::Talk talk;
    talk.id = static_cast<int>(row["id"].As<std::int64_t>());
    talk.title = row["title"].As<std::string>();
    talk.description = row["description"].As<std::string>();
    talk.speaker_id = static_cast<int>(row["speaker_id"].As<std::int64_t>());
    return talk;
}

models::Conference ReadConferenceFromRow(
    const userver::storages::postgres::Row& row
) {
    models::Conference conf;
    conf.id = static_cast<int>(row["id"].As<std::int64_t>());
    conf.title = row["title"].As<std::string>();
    conf.description = row["description"].As<std::string>();
    conf.date = row["conference_date"].As<std::string>();
    return conf;
}

const userver::storages::postgres::Query kInsertTalk{
    "INSERT INTO talks(title, description, speaker_id) "
    "VALUES($1, $2, $3) "
    "RETURNING id, title, description, speaker_id",
    userver::storages::postgres::Query::Name{"insert_talk"},
};

const userver::storages::postgres::Query kSelectAllTalks{
    "SELECT id, title, description, speaker_id "
    "FROM talks "
    "ORDER BY id",
    userver::storages::postgres::Query::Name{"select_all_talks"},
};

const userver::storages::postgres::Query kSelectTalkById{
    "SELECT id, title, description, speaker_id "
    "FROM talks "
    "WHERE id = $1",
    userver::storages::postgres::Query::Name{"select_talk_by_id"},
};

const userver::storages::postgres::Query kInsertConference{
    "INSERT INTO conferences(title, description, conference_date) "
    "VALUES($1, $2, $3::date) "
    "RETURNING id, title, description, conference_date::text AS conference_date",
    userver::storages::postgres::Query::Name{"insert_conference"},
};

const userver::storages::postgres::Query kSelectConferenceById{
    "SELECT id, title, description, conference_date::text AS conference_date "
    "FROM conferences "
    "WHERE id = $1",
    userver::storages::postgres::Query::Name{"select_conference_by_id"},
};

const userver::storages::postgres::Query kInsertConferenceTalk{
    "INSERT INTO conference_talks(conference_id, talk_id) "
    "VALUES($1, $2) "
    "ON CONFLICT DO NOTHING",
    userver::storages::postgres::Query::Name{"insert_conference_talk"},
};

const userver::storages::postgres::Query kSelectConferenceTalks{
    "SELECT t.id, t.title, t.description, t.speaker_id "
    "FROM conference_talks ct "
    "JOIN talks t ON t.id = ct.talk_id "
    "WHERE ct.conference_id = $1 "
    "ORDER BY t.id",
    userver::storages::postgres::Query::Name{"select_conference_talks"},
};

}  // namespace

}  // namespace conference_api::storage

namespace conference_api::storage {

PostgresStorage::PostgresStorage(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db").GetCluster()) {}

const models::User PostgresStorage::CreateUser(
    const std::string& login,
    const std::string& password,
    const std::string& first_name,
    const std::string& last_name,
    const std::string& role
) const {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        kInsertUser,
        login,
        password,
        first_name,
        last_name,
        role
    );

    const auto row = result[0];
    return ReadUserFromRow(row);
}

std::optional<models::User> PostgresStorage::FindUserByLogin(const std::string& login) const {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        kSelectUserByLogin,
        login
    );

    if (result.IsEmpty()) {
        return std::nullopt;
    }

    const auto row = result[0];
    return ReadUserFromRow(row);
}

std::vector<models::User> PostgresStorage::FindUsersByMask(const std::string& mask) const {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        kSearchUsersByMask,
        "%" + mask + "%"
    );

    std::vector<models::User> users;
    users.reserve(result.Size());

    for (const auto& row : result) {
        users.push_back(ReadUserFromRow(row));
    }

    return users;
}

std::optional<models::User> PostgresStorage::ValidateUser(
    const std::string& login,
    const std::string& password
) const {
    const auto user = FindUserByLogin(login);
    if (!user.has_value() || user->password != password) {
        return std::nullopt;
    }
    return user;
}

std::string PostgresStorage::CreateToken(std::int64_t user_id) const {
    const auto token = GenerateToken();
    pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        kInsertToken,
        token,
        user_id
    );
    return token;
}

std::optional<models::User> PostgresStorage::FindUserByToken(const std::string& token) const {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        kSelectUserByToken,
        token
    );

    if (result.IsEmpty()) {
        return std::nullopt;
    }

    const auto row = result[0];
    return ReadUserFromRow(row);
}

const models::Talk PostgresStorage::CreateTalk(
    const std::string& title,
    const std::string& description,
    int speaker_id
) const {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        kInsertTalk,
        title,
        description,
        speaker_id
    );

    return ReadTalkFromRow(result[0]);
}

std::vector<models::Talk> PostgresStorage::GetAllTalks() const {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        kSelectAllTalks
    );

    std::vector<models::Talk> talks;
    talks.reserve(result.Size());

    for (const auto& row : result) {
        talks.push_back(ReadTalkFromRow(row));
    }

    return talks;
}

std::optional<models::Talk> PostgresStorage::FindTalkById(int talk_id) const {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        kSelectTalkById,
        talk_id
    );

    if (result.IsEmpty()) {
        return std::nullopt;
    }

    return ReadTalkFromRow(result[0]);
}

const models::Conference PostgresStorage::CreateConference(
    const std::string& title,
    const std::string& description,
    const std::string& date
) const {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        kInsertConference,
        title,
        description,
        date
    );

    return ReadConferenceFromRow(result[0]);
}

std::optional<models::Conference> PostgresStorage::FindConferenceById(
    int conference_id
) const {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        kSelectConferenceById,
        conference_id
    );

    if (result.IsEmpty()) {
        return std::nullopt;
    }

    return ReadConferenceFromRow(result[0]);
}

bool PostgresStorage::AddTalkToConference(int conference_id, int talk_id) const {
    pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        kInsertConferenceTalk,
        conference_id,
        talk_id
    );

    return true;
}

std::vector<models::Talk> PostgresStorage::GetConferenceTalks(
    int conference_id
) const {
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        kSelectConferenceTalks,
        conference_id
    );

    std::vector<models::Talk> talks;
    talks.reserve(result.Size());

    for (const auto& row : result) {
        talks.push_back(ReadTalkFromRow(row));
    }

    return talks;
}

userver::yaml_config::Schema PostgresStorage::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<userver::components::ComponentBase>(R"(
        type: object
        description: PostgreSQL storage component
        additionalProperties: false
        properties: {}
    )");
}

}