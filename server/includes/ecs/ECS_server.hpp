#pragma once

#include "network/server.hpp"

#include "gameplay/snapshot.hpp"
#include "network/client_manager.hpp"
#include "ecs/entity_manager.hpp"

class ECSServer : public Server {
public:
    using Server::Server;

protected:
    virtual void HandlePacket(struct Packet& packet_to_handle) override;

    virtual void PostProcess() override;

private:
    ClientManager _client_manager = ClientManager();
    EntityManager _entity_manager = EntityManager();

    FrameID current_frame_id = 1;

    void ApplyInput(PlayerID player_id, struct ObjectContext* entity_context);
};