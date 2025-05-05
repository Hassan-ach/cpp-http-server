#include "HttpRequest.hpp"
#include "Utils.hpp"
#include <iostream>
#include <stdexcept>

namespace {
HttpMethod parseMethod(const std::string &methodStr) {
    if (methodStr == "GET")
        return HttpMethod::GET;
    if (methodStr == "POST")
        return HttpMethod::POST;
    return HttpMethod::UNKNOWN;
}
} // namespace

HttpRequest HttpRequest::parse(const std::string &request, int fd) {
    std::vector<std::string> components = Utils::split(request, "\r\n");
    if (components.empty()) {
        throw std::runtime_error("Empty request");
    }

    std::vector<std::string> requestLine = Utils::split(components[0], " ");
    if (requestLine.size() < 3) {
        throw std::runtime_error("Invalid request line");
    }

    RequestLine line{.method = parseMethod(requestLine[0]),
                     .path = requestLine[1],
                     .httpVersion = requestLine[2]};

    HttpRequest req(line, fd);

    // Parse headers
    int bodyIndex = -1;
    for (size_t i = 1; i < components.size(); ++i) {
        size_t end = components[i].find(": ");
        if (end != std::string::npos) {
            std::string key = components[i].substr(0, end);
            std::string value = components[i].substr(end + 2);
            req.headers_[key] = value;
            if (key == "Content-Length") {
                bodyIndex = i + 1;
                break;
            }
        }
    }

    if (req.headers_["Content-Length"] != "0") {
        for (int i = bodyIndex; i < components.size(); ++i) {
            req.body_ += components[i];
        }
    }

    return req;
}
