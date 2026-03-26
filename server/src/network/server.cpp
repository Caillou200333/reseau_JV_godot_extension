#include "network/server.hpp"

#include <iostream>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

Server::Server(int port):_port(port) {}

Server::~Server() { Close(); delete _packet_manager; }

// Getters / Setters
void Server::SetPort(int port) { _port = port; }
int Server::GetPort() const { return _port; }

/////////////////////

// Server loop
void Server::Start(bool set_non_blocking) {
    SetupSocket();

    SetNonBlocking(set_non_blocking);

    double accumulator = 0.;
    TimePoint last_time = Clock::now();

    while (true) {
        auto now = Clock::now();
        std::chrono::duration<double> diff = now - last_time;
        last_time = now;
        accumulator += diff.count();

        ReceivePackets();

        HandlePackets();

        while (accumulator >= dt) {
            accumulator -= dt;
            PostProcess();
        }
    }   
}

void Server::Close() {
    if (_socketId != INVALID_SOCKET) {
        close(_socketId);
        _socketId = INVALID_SOCKET;
    }

    _packet_manager->Empty();
}

void Server::SetupSocket() {
    Close();

    _socketId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (_socketId == INVALID_SOCKET)
        throw std::runtime_error("UDP socket creation failed");

    struct sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(_port);

    if (bind(_socketId, (sockaddr*) &server_addr, sizeof(server_addr)) < 0)
        throw std::runtime_error("Bind failed");

    std::cout << "UDP Server listening on port " << _port << std::endl;
}

void Server::SetNonBlocking(bool set_non_blocking) {
    _non_blocking = set_non_blocking;
    if (!_non_blocking) return;

    int flags = fcntl(_socketId, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL)");
        return;
    }
    if (fcntl(_socketId, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL)");
    }
}

unsigned int Server::ReceivePackets() {
    if (_socketId == INVALID_SOCKET)
        throw std::runtime_error("ReceivePackets failed : UDP socket is invalid");

    // buffer
    static uint8_t buffer[65535];
    static size_t buffer_size = sizeof(buffer);

    // client address
    static struct sockaddr_in client_addr {};
    static socklen_t client_len = sizeof(client_addr);

    static ssize_t bytes_received;

    do {
        bytes_received = recvfrom(
            _socketId,
            buffer,
            buffer_size,
            0,
            (sockaddr*) &client_addr,
            &client_len
        );

        if (bytes_received < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
                break; // Plus rien à lire
            else
                throw std::runtime_error("ReceivePackets failed : recvfrom()");
        }

        // Debug for now (remove buffer size - 1 )
        //buffer[bytes_received] = '\0';
        //std::cout << "Received " << "(size = " << bytes_received << ")" << ": " << buffer << std::endl;
        //

        struct Packet packet_received = Packet(buffer, bytes_received, client_addr);
        _packet_manager->PushPacket(packet_received);
    } while (_non_blocking);

    return _packet_manager->Size();
}

void Server::SendMessage(const BaseMessage& msg_to_send, const struct sockaddr_in& client_addr) const {
    if (_socketId == INVALID_SOCKET)
        throw std::runtime_error("SendMessage failed : UDP socket is invalid");

    // buffer to send
    static uint8_t buffer[65535];
    static size_t buffer_size = sizeof(buffer) - 1;
    
    static socklen_t client_len = sizeof(client_addr);

    MessageSize msg_size = msg_to_send.Serialize(buffer, buffer_size);
    sendto(
            _socketId,
            buffer,
            msg_size,
            0,
            (sockaddr*) &client_addr,
            client_len
        );
}

void Server::HandlePackets() {
    static struct Packet packet_to_handle;

    while (_packet_manager->PopPacket(packet_to_handle)) {
        HandlePacket(packet_to_handle);
    }
}

// Method to override
void Server::HandlePacket(struct Packet& packet_to_handle) { std::cout << "Packet : " << &packet_to_handle << std::endl; }

void Server::PostProcess() {}