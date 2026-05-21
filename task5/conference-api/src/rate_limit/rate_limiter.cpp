#include "rate_limiter.hpp"

namespace conference_api::rate_limit {

long long RateLimitResult::ResetUnixTimestamp() const {
    return std::chrono::duration_cast<std::chrono::seconds>(
        reset_at.time_since_epoch()
    ).count();
}

RateLimiter::RateLimiter(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : LoggableComponentBase(config, context) {}

RateLimitResult RateLimiter::Check(const std::string& key) {
    const auto now = std::chrono::system_clock::now();

    std::lock_guard lock(mutex_);

    auto& window = windows_[key];

    if (window.reset_at <= now) {
        window.count = 0;
        window.reset_at = now + kWindow;
    }

    if (window.count >= kLimit) {
        return RateLimitResult{
            .allowed = false,
            .limit = kLimit,
            .remaining = 0,
            .reset_at = window.reset_at,
        };
    }

    ++window.count;

    return RateLimitResult{
        .allowed = true,
        .limit = kLimit,
        .remaining = kLimit - window.count,
        .reset_at = window.reset_at,
    };
}

void RateLimiter::Clear() {
    std::lock_guard lock(mutex_);
    windows_.clear();
}

}  // namespace conference_api::rate_limit
