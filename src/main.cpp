#include "HttpServer.hpp"
#include <iostream>

int main(int argc, char **argv) {
    // Ensure immediate flushing of output streams
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "Starting HTTP server...\n";

    try {
        ServerConfig config(argc, argv);
        HttpServer server(4221);
        server.start(config);
    } catch (const std::exception &e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
