#include "HttpServer.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>

HttpServer::HttpServer(int port) : port_(port) {}

HttpServer::~HttpServer() = default;

void HttpServer::start(const ServerConfig &config) {
    createSocket();
    bindSocket();
    startListening();

    std::cout << "Server started on port " << port_ << std::endl;
    run(config);
}

void HttpServer::createSocket() {
    serverSocket_ = std::make_unique<TcpSocket>();

    // Set SO_REUSEADDR to avoid "Address already in use" errors
    int reuse = 1;
    if (setsockopt(serverSocket_->fd(), SOL_SOCKET, SO_REUSEADDR, &reuse,
                   sizeof(reuse)) < 0) {
        throw std::runtime_error("Failed to set socket options");
    }
}

void HttpServer::bindSocket() {
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port_);

    if (bind(serverSocket_->fd(), (struct sockaddr *)&serverAddr,
             sizeof(serverAddr)) != 0) {
        throw std::runtime_error("Failed to bind to port " +
                                 std::to_string(port_));
    }
}

void HttpServer::startListening() {
    const int connectionBacklog = 5;
    if (listen(serverSocket_->fd(), connectionBacklog) != 0) {
        throw std::runtime_error("Failed to start listening");
    }
}

void HttpServer::run(const ServerConfig &config) {
    RequestHandler handler(config);
    sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);

    while (true) {
        int clientFd = accept(serverSocket_->fd(),
                              (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientFd < 0) {
            std::cerr << "Failed to accept client connection" << std::endl;
            continue;
        }

        std::thread([this, clientFd, &handler]() {
            handleClient(clientFd, handler);
        }).detach();
    }
}

void HttpServer::handleClient(int clientFd, RequestHandler &handler) {
    TcpSocket clientSocket(clientFd);
    char buffer[1024];

    int bytesRead = clientSocket.recv(buffer, sizeof(buffer));
    if (bytesRead < 0) {
        std::cerr << "Failed to read from client" << std::endl;
        return;
    }

    std::string requestStr(buffer, bytesRead);
    HttpRequest request = HttpRequest::parse(requestStr, clientFd);
    handler.handleRequest(request);
}
