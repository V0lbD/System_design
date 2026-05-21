#include "response_cache.hpp"

#include <mutex>

namespace conference_api::cache {

ResponseCache::ResponseCache(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context
)
    : LoggableComponentBase(config, context) {}

std::optional<std::string> ResponseCache::Get(const std::string& key) const {
    const auto now = std::chrono::steady_clock::now();

    std::lock_guard lock(mutex_);

    auto it = entries_.find(key);
    if (it == entries_.end()) {
        return std::nullopt;
    }

    if (it->second.expires_at <= now) {
        entries_.erase(it);
        return std::nullopt;
    }

    return it->second.value;
}

void ResponseCache::Put(
    const std::string& key,
    std::string value,
    std::chrono::seconds ttl
) {
    std::lock_guard lock(mutex_);

    entries_[key] = Entry{
        .value = std::move(value),
        .expires_at = std::chrono::steady_clock::now() + ttl,
    };
}

void ResponseCache::Invalidate(const std::string& key) {
    std::lock_guard lock(mutex_);
    entries_.erase(key);
}

void ResponseCache::Clear() {
    std::lock_guard lock(mutex_);
    entries_.clear();
}

}  // namespace conference_api::cache
