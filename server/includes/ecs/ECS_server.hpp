#pragma once

#include "network/server.hpp"

#include "network/client_manager.hpp"
#include "ecs/entity_manager.hpp"

class ECSServer : public Server {
public:
    using Server::Server;

protected:
    virtual void HandlePacket(struct Packet& packet_to_handle) override;

private:
    ClientManager _client_manager = ClientManager();
    EntityManager _entity_manager = EntityManager();
};