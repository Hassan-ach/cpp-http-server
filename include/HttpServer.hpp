#pragma once

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "RequestHandler.hpp"
#include "TcpSocket.hpp"
#include <memory>
#include <thread>
#include <unordered_map>

class HttpServer {
  public:
    explicit HttpServer(int port);
    ~HttpServer();

    void start(const ServerConfig &config);

  private:
    int port_;
    std::unique_ptr<TcpSocket> serverSocket_;

    void createSocket();
    void bindSocket();
    void startListening();
    void run(const ServerConfig &config);
    void handleClient(int clientFd, RequestHandler &handler);
};
