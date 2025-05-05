#pragma once

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <functional>
#include <string>
#include <unordered_map>

struct ServerConfig {
    std::string directory;

    ServerConfig(int argc, char **argv);
};

class RequestHandler {
  public:
    using EndpointHandler = std::function<HttpResponse(const HttpRequest &)>;

    explicit RequestHandler(const ServerConfig &config);

    void handleRequest(const HttpRequest &request);

  private:
    ServerConfig config_;
    // std::unordered_map<std::string, EndpointHandler> endpoints_;

    // void initializeEndpoints();
    HttpResponse handleRoot() const;
    HttpResponse handleEcho(const std::string &content) const;
    HttpResponse handleUserAgent(const std::string &userAgent) const;
    HttpResponse handleGetFiles(const std::string &fileName) const;
    HttpResponse handlePostFiles(const std::string &fileName,
                                 const std::string &content) const;
    HttpResponse handleNotFound() const;
    void sendResponse(int clientFd, const HttpResponse &response) const;
};
