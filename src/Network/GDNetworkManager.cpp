#include "Network/GDNetworkManager.h"

#include <iostream>
#include <godot_cpp/core/class_db.hpp>

GDNetworkManager::GDNetworkManager() {}

GDNetworkManager::~GDNetworkManager() { Close(); }

void GDNetworkManager::SetNonBlocking(socket_t sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl(F_GETFL)");
        return;
    }
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl(F_SETFL)");
    }
}

void GDNetworkManager::Close(void) {
    if (udp_socket != INVALID_SOCKET) {
        close(udp_socket);
        udp_socket = INVALID_SOCKET;
    }
}

void GDNetworkManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("Bind", "port"), &GDNetworkManager::Bind);
	ClassDB::bind_method(D_METHOD("Send", "ip", "port", "data"), &GDNetworkManager::Send);
	ClassDB::bind_method(D_METHOD("Receive"), &GDNetworkManager::Receive);

    ADD_SIGNAL(MethodInfo("packet_received",
        PropertyInfo(Variant::STRING, "sender_ip"),
        PropertyInfo(Variant::INT, "sender_port"),
        PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data")
    ));
}

bool GDNetworkManager::Bind(int port) {
    Close(); //Juste in case

    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if (udp_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }

    SetNonBlocking(udp_socket);

    std::cerr << "Sockets successfully initialized" << std::endl;

    struct sockaddr_in server_address {};

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // inet_addr("192.168.11.129"); // this is address of host which I want to send the socket
    server_address.sin_port = htons(port);

    if (bind(udp_socket, (const struct sockaddr*) &server_address, sizeof(struct sockaddr_in)) < 0) {
        std::cerr << "Failed to bind socket : " << strerror(errno) << std::endl;
        Close();
        return false;
    }

    std::cerr << "UDP Socket successfully binded on port : " << port << std::endl;
    return true;
}

void GDNetworkManager::Send(String ip, int port, PackedByteArray data) {
    if (udp_socket == INVALID_SOCKET) return;

    struct sockaddr_in dest_address {};

    dest_address.sin_family = AF_INET;
    dest_address.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip.utf8().get_data(), &dest_address.sin_addr) <= 0) {
        UtilityFunctions::printerr("Invalid IP: ", ip);
        return;
    }

    //inet_pton(AF_INET, ip, dest_address.sin_addr.s_addr);

    int sent_bytes = sendto(udp_socket, (const char*)data.ptr(), data.size(), 0, (struct sockaddr*) &dest_address, sizeof(struct sockaddr_in));
}

// Check for incoming packets (Call this in _process)
void GDNetworkManager::Receive() {
    if (udp_socket == INVALID_SOCKET) return;

    char buffer[65535];
    sockaddr_in sender_address;
    socklen_t address_len = sizeof(struct sockaddr_in);
    int received_bytes;

    while (true) {
        int received_bytes = recvfrom(
            udp_socket,
            buffer,
            sizeof(buffer),
            0,
            (sockaddr*)&sender_address,
            &address_len
        );

        if (received_bytes < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                break; // Plus rien à lire
            } else {
                perror("recvfrom()");
                break;
            }
        }

        PackedByteArray received_data;
        received_data.resize(received_bytes);
        memcpy(received_data.ptrw(), buffer, received_bytes);

        char sender_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sender_address.sin_addr, sender_ip, INET_ADDRSTRLEN);
        int sender_port = ntohs(sender_address.sin_port);

        emit_signal("packet_received", String(sender_ip), sender_port, received_data);
    }

    // memcpy(received_data, buffer, received_bytes);
}