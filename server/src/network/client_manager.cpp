#include "network/client_manager.hpp"

char ClientManager::GenerateClientName() {
    return _next_client_name ++;
}

PlayerID ClientManager::GenerateClientID() {
    return _next_client_id ++;
}

const struct ClientConnection& ClientManager::AddClient(const sockaddr_in &addr, NetworkID network_id) {
    clients.insert({addr, GenerateClientName(), GenerateClientID(), network_id});
    return GetClient(addr);
}

void ClientManager::RemoveClient(const sockaddr_in &addr) {
    ClientConnection tmp{addr};
    clients.erase(tmp);
}

const struct ClientConnection& ClientManager::GetClient(const sockaddr_in &addr) const {
    ClientConnection tmp{addr};
    auto it = clients.find(tmp);
    if (it != clients.end()) return *it;
    return ClientManager::EmptyClient;
}

bool ClientManager::HasClient(const sockaddr_in &addr) const {
    ClientConnection tmp{addr};
    return clients.find(tmp) != clients.end();
}

const std::unordered_set<ClientConnection>& ClientManager::AllClients() const {
    return clients;
}
