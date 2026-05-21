#pragma once

#include <chrono>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

#include <userver/components/loggable_component_base.hpp>

namespace conference_api::rate_limit {

struct RateLimitResult {
    bool allowed{false};
    int limit{0};
    int remaining{0};
    std::chrono::system_clock::time_point reset_at;

    long long ResetUnixTimestamp() const;
};

class RateLimiter final : public userver::components::LoggableComponentBase {
public:
    static constexpr std::string_view kName = "rate-limiter";

    RateLimiter(
        const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& context
    );

    RateLimitResult Check(const std::string& key);

    void Clear();

private:
    struct Window {
        int count{0};
        std::chrono::system_clock::time_point reset_at;
    };

    static constexpr int kLimit = 5;
    static constexpr std::chrono::seconds kWindow{60};

    mutable std::mutex mutex_;
    std::unordered_map<std::string, Window> windows_;
};

}  // namespace conference_api::rate_limit
