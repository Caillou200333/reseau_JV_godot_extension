#include "network/packet_manager.hpp"

#include <cstring>

// Packets
Packet::Packet():data(nullptr),size(0) {}

Packet::Packet(const uint8_t* data_, unsigned int size_, const struct sockaddr_in& addr):size(size_),sender_addr(addr) {
    if (size <= 0) {
        data = nullptr;
        return;
    }

    data = new uint8_t[size];
    std::memcpy(data, data_, size);
}

Packet::Packet(const struct Packet& packet):Packet(packet.data, packet.size, packet.sender_addr) {}

Packet& Packet::operator=(const struct Packet& packet) {
    if (this == &packet)
        return *this;

    if (data) delete[] data;

    size = packet.size;
    sender_addr = packet.sender_addr;
    
    if (size <= 0) {
        data = nullptr;
        return *this;
    }
    
    data = new uint8_t[size];
    std::memcpy(data, packet.data, size);

    return *this;
}

Packet::~Packet() { if (data) delete[] data; }

// PacketManager

PacketManager::~PacketManager() { Empty(); }

void PacketManager::PushPacket(struct Packet& packet) { _queue.push(packet); }

bool PacketManager::PopPacket(Packet& outPacket) {
    if (_queue.empty()) return false;

    outPacket = _queue.front();
    _queue.pop();
    return true;
}

bool PacketManager::IsEmpty() const {
    return _queue.empty();
}

void PacketManager::Empty() {
    while (!_queue.empty()) {
        _queue.pop();
    }
}

unsigned int PacketManager::Size() const {
    return _queue.size();
}