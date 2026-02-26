#include "network/client_manager.hpp"

char ClientManager::GenerateClientName() {
    return _next_client_name ++;
}

const struct ClientConnection& ClientManager::AddClient(const sockaddr_in &addr) {
    clients.insert({addr, GenerateClientName()});
    return GetClient(addr);
}

void ClientManager::RemoveClient(const sockaddr_in &addr) {
    ClientConnection tmp{addr, 'A'};
    clients.erase(tmp);
}

const struct ClientConnection& ClientManager::GetClient(const sockaddr_in &addr) const {
    ClientConnection tmp{addr, 'A'};
    auto it = clients.find(tmp);
    if (it != clients.end()) return *it;
    return ClientManager::EmptyClient;
}

bool ClientManager::HasClient(const sockaddr_in &addr) const {
    ClientConnection tmp{addr, 'A'};
    return clients.find(tmp) != clients.end();
}

const std::unordered_set<ClientConnection>& ClientManager::AllClients() const {
    return clients;
}
