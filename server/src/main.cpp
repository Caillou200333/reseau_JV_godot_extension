#include "ecs/ECS_server.hpp"
#include "message_factory.hpp"

#include <iostream>

int main(int argc, char** argv) {
    int opt;
    int port = 8080;
    int framerate = 60;

    while ((opt = getopt(argc, argv, "p:f:")) != -1) {
        switch (opt) {
            case 'p':
                port = std::atoi(optarg);
                break;
            case 'f':
                framerate = std::atoi(optarg);
                break;
            default:
                std::cerr << "Usage: " << argv[0]
                        << " [-p port] [-t tick_rate]\n";
                return 1;
        }
    }

    float fixed_frame_length = 1.0f / static_cast<float>(framerate);
    // RegisterAll the kind of messages beforehand
    MessageFactory::RegisterAll();

    try {
        ECSServer server(port, fixed_frame_length);
        server.Start();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
