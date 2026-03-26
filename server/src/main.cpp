#include "ecs/ECS_server.hpp"
#include "message_factory.hpp"

#include <iostream>

int main() {
    // RegisterAll the kind of messages beforehand
    MessageFactory::RegisterAll();

    try {
        ECSServer server(8080);
        server.Start();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
