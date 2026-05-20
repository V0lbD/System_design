#pragma once

#include <string>

#include <userver/formats/json/value_builder.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/http/http_status.hpp>

namespace conference_api::utils {

inline std::string MakeJsonMessageResponse(
    const userver::server::http::HttpRequest& request,
    userver::server::http::HttpStatus status,
    const std::string& message
) {
    auto& response = request.GetHttpResponse();
    response.SetStatus(status);
    response.SetContentType("application/json");

    userver::formats::json::ValueBuilder body;
    body["message"] = message;
    return userver::formats::json::ToString(body.ExtractValue());
}

inline std::string BadRequest(
    const userver::server::http::HttpRequest& request,
    const std::string& message
) {
    return MakeJsonMessageResponse(
        request,
        userver::server::http::HttpStatus::kBadRequest,
        message
    );
}

inline std::string Unauthorized(
    const userver::server::http::HttpRequest& request,
    const std::string& message
) {
    return MakeJsonMessageResponse(
        request,
        userver::server::http::HttpStatus::kUnauthorized,
        message
    );
}

inline std::string Forbidden(
    const userver::server::http::HttpRequest& request,
    const std::string& message
) {
    return MakeJsonMessageResponse(
        request,
        userver::server::http::HttpStatus::kForbidden,
        message
    );
}

inline std::string NotFound(
    const userver::server::http::HttpRequest& request,
    const std::string& message
) {
    return MakeJsonMessageResponse(
        request,
        userver::server::http::HttpStatus::kNotFound,
        message
    );
}

inline std::string Conflict(
    const userver::server::http::HttpRequest& request,
    const std::string& message
) {
    return MakeJsonMessageResponse(
        request,
        userver::server::http::HttpStatus::kConflict,
        message
    );
}

}