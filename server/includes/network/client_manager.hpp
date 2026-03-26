#pragma once

#include "gameplay/common_tools.hpp"

#include <unordered_set>
#include <netinet/in.h>   // sockaddr_in

struct ClientConnection {
    struct sockaddr_in address;
    char client_name = 'A';
    PlayerID client_id = INVALID_ID;
    NetworkID controlled_entity = -1; 
    
    // Comparaison basée sur l'adresse IP et le port
    bool operator==(const ClientConnection &other) const {
        return address.sin_addr.s_addr == other.address.sin_addr.s_addr &&
                address.sin_port == other.address.sin_port;
    }
};

// Hash basé sur IP + port pour unordered_set
namespace std {
    template <>
    struct hash<ClientConnection> {
        std::size_t operator()(const ClientConnection &c) const {
            return std::hash<uint32_t>()(c.address.sin_addr.s_addr) ^
                    (std::hash<uint16_t>()(c.address.sin_port) << 1);
        }
    };
}

class ClientManager {
private:
    std::unordered_set<ClientConnection> clients;

    inline static const struct ClientConnection EmptyClient = {{}, '0', INVALID_ID, 0};

    char _next_client_name = 'A';

    char GenerateClientName();

    PlayerID _next_client_id = 1;

    PlayerID GenerateClientID();

public:
    ClientManager() = default;

    const struct ClientConnection& AddClient(const sockaddr_in &addr, NetworkID network_id);

    void RemoveClient(const sockaddr_in &addr);

    const struct ClientConnection& GetClient(const sockaddr_in &addr) const;

    bool HasClient(const sockaddr_in &addr) const;

    const std::unordered_set<ClientConnection>& AllClients() const;
};
