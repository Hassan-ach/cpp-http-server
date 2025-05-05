#include "RequestHandler.hpp"
#include "TcpSocket.hpp"
#include "Utils.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

ServerConfig::ServerConfig(int argc, char **argv) {
    if (argc > 2 && std::string(argv[1]) == "--dir") {
        directory = argv[2];
    }
}

RequestHandler::RequestHandler(const ServerConfig &config) : config_(config) {
    // initializeEndpoints();
}

void RequestHandler::handleRequest(const HttpRequest &request) {
    std::vector<std::string> urlParams = Utils::split(request.line().path, "/");
    HttpResponse response;

    if (!urlParams.empty()) {
        if (urlParams.size() == 2 && urlParams[0] == "echo") {
            response = handleEcho(urlParams[1]);
        } else if (urlParams[0] == "user-agent") {
            response = handleUserAgent(request.headers().at("User-Agent"));
        } else if (urlParams.size() == 2 && urlParams[0] == "files") {
            if (request.line().method == HttpMethod::GET) {
                response = handleGetFiles(urlParams[1]);
            }
            if (request.line().method == HttpMethod::POST) {
                response = handlePostFiles(urlParams[1], request.body());
            }

        } else {
            response = handleNotFound();
        }
    } else {
        response = handleRoot();
    }

    sendResponse(request.clientFd(), response);
}

// void RequestHandler::initializeEndpoints() {
//     endpoints_["/"] = [this](const HttpRequest &) { return handleRoot(); };
//     endpoints_["echo"] = [this](const HttpRequest &req) {
//         auto params = Utils::split(req.line().path, "/");
//         return handleEcho(params.size() > 1 ? params[1] : "");
//     };
//     endpoints_["getFile"] = [this](const HttpRequest &req) {
//         auto params = Utils::split(req.line().path, "/");
//         return handleGetFiles(params.size() > 1 ? params[1] : "");
//     };
// }

HttpResponse RequestHandler::handleRoot() const {
    HttpResponse response;
    response.setStatus("200 OK");
    return response;
}

HttpResponse RequestHandler::handleEcho(const std::string &content) const {
    HttpResponse response;
    response.setStatus("200 OK");
    response.setHeader("Content-Type", "text/plain");
    response.setBody(content);
    return response;
}

HttpResponse
RequestHandler::handleUserAgent(const std::string &userAgent) const {
    return handleEcho(userAgent);
}

HttpResponse RequestHandler::handleGetFiles(const std::string &fileName) const {
    HttpResponse response;

    try {
        // for (const auto &entry : fs::directory_iterator(config_.directory)) {
        //     std::vector<std::string> pathParts =
        //         Utils::split(entry.path().string(), "/");
        //     if (pathParts.back() == fileName) {
        //         std::ifstream file(entry.path(), std::ios::binary);
        //         if (file) {
        //             std::string
        //             content((std::istreambuf_iterator<char>(file)),
        //                                 std::istreambuf_iterator<char>());
        //
        //             response.setStatus("200 OK");
        //             response.setHeader("Content-Type",
        //                                "application/octet-stream");
        //             response.setBody(content);
        //             return response;
        //         }
        //     }
        // }
        fs::path filePath = fs::path(config_.directory) / fileName;

        // Check if file exists and is regular file
        if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
            std::ifstream file(filePath, std::ios::binary);
            if (file) {
                // Read file content
                std::string content((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());
                file.close();

                // Set successful response
                response.setStatus("200 OK");
                response.setHeader("Content-Type",
                                   Utils::getMimeType(filePath));
                response.setBody(content);
                return response;
            }
        }
        response.setStatus("404 Not Found");
        return response;
    } catch (const std::exception &) {
        response.setStatus("404 Not Found");
        return response;
    }
}

HttpResponse RequestHandler::handlePostFiles(const std::string &fileName,
                                             const std::string &content) const {
    //
    HttpResponse response;
    try {
        fs::path filePath = fs::path(config_.directory) / fileName;

        if (fs::exists(filePath)) {
            //
            response.setStatus("505 Server Error");
            response.setBody("File aredy exist! ");
            return response;
        } else {
            std::ofstream file(filePath, std::ios::binary);
            if (file) {
                // file << content;
                file.write(content.c_str(), content.size());
                file.close();
                response.setStatus("201 Created");
                return response;
            }
        }
    } catch (const std::exception &) {
        //
        response.setStatus("505 Server Error");
        response.setBody("File aredy exist! ");
        return response;
    }

    return response;
}

HttpResponse RequestHandler::handleNotFound() const {
    HttpResponse response;
    response.setStatus("404 Not Found");
    return response;
}

void RequestHandler::sendResponse(int clientFd,
                                  const HttpResponse &response) const {
    std::string responseStr = response.toString();
    TcpSocket clientSocket(clientFd);
    ssize_t bytesSent =
        clientSocket.send(responseStr.c_str(), responseStr.size());

    if (bytesSent != static_cast<ssize_t>(responseStr.size())) {
        std::cerr << "Failed to send complete response. Sent " << bytesSent
                  << " of " << responseStr.size() << " bytes." << std::endl;
    }
}
