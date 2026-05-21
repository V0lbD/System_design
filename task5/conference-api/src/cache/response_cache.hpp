#pragma once

#include <chrono>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

#include <userver/components/loggable_component_base.hpp>

namespace conference_api::cache {

class ResponseCache final : public userver::components::LoggableComponentBase {
public:
    static constexpr std::string_view kName = "response-cache";

    ResponseCache(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );

    std::optional<std::string> Get(const std::string& key) const;

    void Put(
        const std::string& key,
        std::string value,
        std::chrono::seconds ttl
    );

    void Invalidate(const std::string& key);

    void Clear();

private:
    struct Entry {
        std::string value;
        std::chrono::steady_clock::time_point expires_at;
    };

    mutable std::mutex mutex_;
    mutable std::unordered_map<std::string, Entry> entries_;
};

}  // namespace conference_api::cache
