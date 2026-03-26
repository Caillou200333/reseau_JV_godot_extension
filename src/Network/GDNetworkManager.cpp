#include "Network/GDNetworkManager.h"

#include "Network/Messages/GDHELOMessage.h"
#include "Network/Messages/GDGameplayMessage.h"
#include "Network/Messages/GDTextMessage.h"

#include "messages/rtt_message.hpp"

#include <iostream>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

GDNetworkManager::GDNetworkManager() {}

GDNetworkManager::~GDNetworkManager() { Close(); }

void GDNetworkManager::SetNonBlocking(socket_t sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) {
        UtilityFunctions::printerr("fcntl(F_GETFL) failed");
        return;
    }
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        UtilityFunctions::printerr("fcntl(F_SETFL) failed");
    }
}

void GDNetworkManager::Close(void) {
    if (udp_socket != INVALID_SOCKET) {
        StopRTTLoop();
        close(udp_socket);
        udp_socket = INVALID_SOCKET;
    }
}

void GDNetworkManager::_bind_methods() {
	ClassDB::bind_method(D_METHOD("Bind", "port"), &GDNetworkManager::Bind);
	ClassDB::bind_method(D_METHOD("Send", "ip", "port", "messsage"), &GDNetworkManager::Send);
	ClassDB::bind_method(D_METHOD("Receive"), &GDNetworkManager::Receive);
	ClassDB::bind_method(D_METHOD("StartRTT", "ip", "port"), &GDNetworkManager::StartRTTLoop);

    ADD_SIGNAL(MethodInfo("packet_received",
        PropertyInfo(Variant::STRING, "sender_ip"),
        PropertyInfo(Variant::INT, "sender_port"),
        PropertyInfo(Variant::OBJECT, "message", PROPERTY_HINT_RESOURCE_TYPE, "GDBaseMessage")
    ));
}

bool GDNetworkManager::Bind(int port) {
    Close(); //Juste in case

    udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if (udp_socket == INVALID_SOCKET) {
        UtilityFunctions::printerr("Failed to create socket");
        return false;
    }

    SetNonBlocking(udp_socket);

    UtilityFunctions::print("Sockets successfully initialized");

    struct sockaddr_in server_address {};

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // inet_addr("192.168.11.129"); // this is address of host which I want to send the socket
    server_address.sin_port = htons(port);

    if (bind(udp_socket, (const struct sockaddr*) &server_address, sizeof(struct sockaddr_in)) < 0) {
        UtilityFunctions::printerr("Failed to bind socket: ", strerror(errno));
        Close();
        return false;
    }

    UtilityFunctions::print("UDP Socket successfully binded on port: ", port);
    return true;
}

void GDNetworkManager::SendLogic(const char* ip, int port, const BaseMessage* msg) {
    if (udp_socket == INVALID_SOCKET) return;

    static uint8_t buffer[65535];
    static size_t buffer_size = sizeof(buffer);

    static struct sockaddr_in dest_address {};
    static socklen_t dest_len = sizeof(dest_address);

    dest_address.sin_family = AF_INET;
    dest_address.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip, &dest_address.sin_addr) <= 0) {
        UtilityFunctions::printerr("Invalid IP: ", ip);
        return;
    }

    MessageSize msg_size = msg->Serialize(buffer, buffer_size);
    
    int sent_bytes = sendto(
            udp_socket,
            buffer,
            msg_size,
            0,
            (struct sockaddr*) &dest_address,
            dest_len
        );

    if (sent_bytes < 0) {
        UtilityFunctions::printerr("Failed to send packet: ", strerror(errno));
        return;
    }
}

void GDNetworkManager::Send(String ip, int port, const Ref<GDBaseMessage>& message) {
    SendLogic(ip.utf8().get_data(), port, message->GetMessage().get());
}

// Check for incoming packets (Call this in _process)
void GDNetworkManager::Receive() {
    if (udp_socket == INVALID_SOCKET) return;

    static uint8_t buffer[65535];
    static size_t buffer_size = sizeof(buffer);

    static struct sockaddr_in sender_address {};
    static socklen_t address_len = sizeof(struct sockaddr_in);
    
    static int received_bytes;

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
                UtilityFunctions::printerr("recvfrom() failed: ", strerror(errno));
                break;
            }
        }

        char sender_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sender_address.sin_addr, sender_ip, INET_ADDRSTRLEN);
        int sender_port = ntohs(sender_address.sin_port);

        MessageType type = BaseMessage::PeekType(buffer);

        Ref<GDBaseMessage> msg_received = nullptr;
        switch (type) {
        case HELO: {
            Ref<GDHELOMessage> helo = memnew(GDHELOMessage);
            msg_received = helo;
            break;
        }
        case GAMEPLAY: {
            Ref<GDGameplayMessage> gameplay = memnew(GDGameplayMessage);
            msg_received = gameplay;
            break;
        }
        case TEXT: {
            Ref<GDTextMessage> txt = memnew(GDTextMessage);
            msg_received = txt;
            break;
        }
        case RTT: {
            RTTMessage rtt_msg;
            rtt_msg.Deserialize(buffer, received_bytes);
            uint64_t rtt = rtt_msg.CalculateRTT();
            UtilityFunctions::print("Current RTT : ", rtt);
            break;
        }
        default:
            UtilityFunctions::printerr("Unknown message type received");
            break;
        }

        if (msg_received.is_valid()) { // Asert if is valid
            msg_received->GetMessage().get()->Deserialize(buffer, received_bytes);
            emit_signal("packet_received", String(sender_ip), sender_port, msg_received);
        }
    }
}

#include <thread>
#include <chrono>
#include <atomic>

std::atomic<bool> _rtt_running{false};
std::thread _rtt_thread;

void GDNetworkManager::RTTLoopFunction(const char* ip, int port) {
    while (_rtt_running) {
        RTTMessage rtt_msg;
        rtt_msg.Init();
        SendLogic(ip, port, &rtt_msg);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // each second
    }
}

void GDNetworkManager::StartRTTLoop(String ip, int port) {
    if (_rtt_running) return; // already running

    _rtt_running = true;
    _rtt_thread = std::thread([this, ip, port] {
        RTTLoopFunction(ip.utf8().get_data(), port);
    });
}


void GDNetworkManager::StopRTTLoop() {
    _rtt_running = false;
    if (_rtt_thread.joinable())
        _rtt_thread.join();
}