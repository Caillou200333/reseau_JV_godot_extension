#include "ecs/ECS_server.hpp"

#include "message_factory.hpp"

#include "messages/base_message.hpp"
#include "messages/gameplay_message.hpp"
#include "messages/helo_message.hpp"
#include "messages/input_message.hpp"
#include "messages/text_message.hpp"
#include "messages/rtt_message.hpp"

#include <iostream>

void ECSServer::HandlePacket(struct Packet& packet_to_handle) {
    MessageType type = BaseMessage::PeekType(packet_to_handle.data);

    std::unique_ptr<BaseMessage> base_msg = MessageFactory::Create(type);
    if (!base_msg) {
        std::cerr << "[Server] : Unknown message type received." << std::endl;
        return;
    }

    if (base_msg->GetType() == INPUT) {
        PlayerID client_id = _client_manager.GetClient(packet_to_handle.sender_addr).client_id;
        InputMessage::SetPlayerID(client_id);
    }
    base_msg->Deserialize(packet_to_handle.data, packet_to_handle.size);

    switch (base_msg->GetType()) {
    case HELO: {
        auto* msg = dynamic_cast<HELOMessage*>(base_msg.get());
        if (!msg) return;

        if (_client_manager.HasClient(packet_to_handle.sender_addr)) {
            // Client already connected
            const struct ClientConnection& client = _client_manager.GetClient(packet_to_handle.sender_addr);
            std::cout << "[Server] : Client " << client.client_name << " is already connected." << std::endl;
            return;
        }

        struct ObjectContext* entity_context = _entity_manager.CreateEntity(ClassID::PLAYER_CLASS);
        struct Position& pos = entity_context->GetPosition();
        pos.x = 100 + entity_context->network_id*100;
        pos.y = 100 + entity_context->network_id*100;
        const struct ClientConnection& new_client = _client_manager.AddClient(packet_to_handle.sender_addr, entity_context->network_id);

        std::cout << "[Server] : Client " << new_client.client_name << " connected. Spawning Entity ID " << entity_context->network_id << "." << std::endl;
        Replicate(entity_context);
        break;
    }
    case HANDSHAKE:
        break;
    case GAMEPLAY:
        break;
    case INPUT: {
        const struct ClientConnection& client = _client_manager.GetClient(packet_to_handle.sender_addr);
        PlayerID client_id = client.client_id;
        struct ObjectContext* entity_context = _entity_manager.GetEntity(client.controlled_entity);

        if (entity_context)
            ApplyInput(client_id, entity_context);
        break;
    }
    case TEXT: {
        auto* msg = dynamic_cast<TextMessage*>(base_msg.get());
        if (!msg) return;

        std::cout << "[";
        if (_client_manager.HasClient(packet_to_handle.sender_addr))
            std::cout << "Client " << _client_manager.GetClient(packet_to_handle.sender_addr).client_name;
        else
            std::cout << "Unkonwn Client";
        std::cout << "] : " << msg->GetText() << std::endl;

        TextMessage response = TextMessage("Pong", 4);
        SendMessage(response, packet_to_handle.sender_addr);
        break;
    }
    case RTT: {
        auto* msg = dynamic_cast<RTTMessage*>(base_msg.get());
        if (!msg) return;

        RTTMessage response = *msg;
        SendMessage(response, packet_to_handle.sender_addr);
        break;
    }
    case BYE:
        break;
    default:
        break;
    }
}

void ECSServer::PostProcess() {
    auto& entities = _entity_manager.AllEntities();

    for (auto& [network_id, entity_ctx] : entities) {
        auto& pos = entity_ctx.GetPosition();
        auto& vel = entity_ctx.GetVelocity();

        pos.x += vel.x * dt;
        pos.y += vel.y * dt;

        Replicate(&entity_ctx);
    }
}

void ECSServer::Replicate(const struct ObjectContext* entity_context) const {
    const struct Position& pos = entity_context->GetPosition();
    GameplayMessage msg_to_send = GameplayMessage(entity_context->network_id, entity_context->class_id, pos.x, pos.y);

    for (const auto& client : _client_manager.AllClients()) {
        SendMessage(msg_to_send, client.address);
    }
}

void ECSServer::ApplyInput(PlayerID player_id, struct ObjectContext* entity_context) {
    const struct Input* input = nullptr;
    struct Velocity& vel = entity_context->GetVelocity();

    while ((input = InputMessage::GetNextInput(player_id))) {
        vel.x = 0.;
        vel.y = 0.;

        const double speed = 200.;

        if (input->HasKey(InputID::INPUT_UP)) vel.y -= speed;
        if (input->HasKey(InputID::INPUT_DOWN)) vel.y += speed;
        if (input->HasKey(InputID::INPUT_LEFT)) vel.x -= speed;
        if (input->HasKey(InputID::INPUT_RIGHT)) vel.x += speed;
    }
}