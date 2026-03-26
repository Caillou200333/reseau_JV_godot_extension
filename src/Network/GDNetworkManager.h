#pragma once 

#include <godot_cpp/classes/node.hpp>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t;
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    typedef int socket_t;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

using namespace godot;

#include "Network/Messages/GDBaseMessage.h"

class GDNetworkManager : public Node {
    GDCLASS(GDNetworkManager, Node)

private:
    socket_t udp_socket = INVALID_SOCKET;
    
    // Internal helper to set socket to non-blocking mode
    void SetNonBlocking(socket_t sock);
        
    // Internal helper to close socket safely on all platforms
    void Close();

    // Send the given message to given ip and port
    void SendLogic(const char* ip, int port, const BaseMessage* msg);

    // RTT loop function
    void RTTLoopFunction(const char* ip, int port);

protected:
    static void _bind_methods();

public:
    GDNetworkManager();
    ~GDNetworkManager();

    //void _process(double delta) override;

    // Bind to a port to receive data (Server or P2P Peer)
    bool Bind(int port);

    // Send a packet to a specific IP/Port
    void Send(String ip, int port, const Ref<GDBaseMessage>& message);

    // Check for incoming packets (Call this in _process)
    void Receive();

    // Start the RTT-request loop
    void StartRTTLoop(String ip, int port);
    
    // Stop the RTT-request loop
    void StopRTTLoop();
};