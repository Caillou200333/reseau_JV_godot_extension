#include "ecs/ECS_server.hpp"

#include "messages/gameplay_message.hpp"
#include "messages/helo_message.hpp"
#include "messages/text_message.hpp"

#include <iostream>

void ECSServer::HandlePacket(struct Packet& packet_to_handle) {
    MessageType type = BaseMessage::PeekType(packet_to_handle.data);
    switch (type) {
    case HELO: {
        HELOMessage msg = HELOMessage();
        msg.Deserialize(packet_to_handle.data, packet_to_handle.size);

        if (_client_manager.HasClient(packet_to_handle.sender_addr)) {
            // Client already connected
            const struct ClientConnection& client = _client_manager.GetClient(packet_to_handle.sender_addr);
            std::cout << "[Server] : Client " << client.client_name << " is already connected." << std::endl;
            break;
        }

        const struct ClientConnection& new_client = _client_manager.AddClient(packet_to_handle.sender_addr);
        const struct ObjectContext& entity_context = _entity_manager.CreateEntity(ClassID::PLAYER_CLASS);

        std::cout << "[Server] : Client " << new_client.client_name << " connected. Spawning Entity ID " << entity_context.network_id << "." << std::endl;
        GameplayMessage msg_to_send = GameplayMessage(entity_context.network_id, entity_context.class_id, 100 + entity_context.network_id*100, 100 + entity_context.network_id*100);

        // Boucle sur tous les clients
        for (const auto& client : _client_manager.AllClients()) {
            SendMessage(msg_to_send, client.address);
        }
        break;
    }
    case GAMEPLAY:
        break;
    case TEXT: {
        TextMessage msg = TextMessage();
        msg.Deserialize(packet_to_handle.data, packet_to_handle.size);
        std::cout << "[";
        if (_client_manager.HasClient(packet_to_handle.sender_addr))
            std::cout << "Client " << _client_manager.GetClient(packet_to_handle.sender_addr).client_name;
        else
            std::cout << "Unkonwn Client";
        std::cout << "] : " << msg.GetText() << std::endl;
        TextMessage response = TextMessage("Pong", 4);
        SendMessage(response, packet_to_handle.sender_addr);
        break;
    }
    default:
        break;
    }
}