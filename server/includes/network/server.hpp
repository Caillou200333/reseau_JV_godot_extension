#pragma once

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

#include "network/packet_manager.hpp"
#include "messages/base_message.hpp"

class Server {
public:
    /**
    * @brief Create a server with a designated port
    */
    Server(int port);
    ~Server();

    /** 
    * @brief Set/Get the port.
    */
    void SetPort(int port);
    int GetPort() const;

    /** 
    * @brief Start the server : bind the socket on the given port and start the game loop
    * Can be set as blocking/non blocking for the listening part.
    */
    void Start(bool set_non_blocking = true);

private:
    int _port;
    bool _non_blocking;
    socket_t _socketId = INVALID_SOCKET;

    PacketManager* _packet_manager = new PacketManager(); // store the received packets to be treated
    
    /**
    * @brief Close the currently running socket.
    */
    void Close();

    /** 
    * @brief Bind to the socket to the given port.
    */
    void SetupSocket();

    /** 
    * @brief Set the initialized socket as non a blocking one.
    */
    void SetNonBlocking(bool set_non_blocking);

    /**
    * @brief Try to receive a packet on the server port.
    * Attempts to read all available UDP packets from the socket.
    * Can be defined as blocking/non-blocking from the SetNonBlocking function.
    * If is blocking, only read one packet.
    *
    * @return the number of packets received
    */
    unsigned int ReceivePackets();

    /** 
    * @brief Handle the received packet.
    */
    void HandlePackets();

protected:
    /**
    * @brief Send a packet to the given client.
    */
    void SendMessage(const BaseMessage& msg_to_send, const struct sockaddr_in& client_addr) const;

    /////// Virtual method to implement in order to determine the server comportement
    /**
     * @brief Handler function to implement to specify the comportment when handling a packet.
     */
    virtual void HandlePacket(struct Packet& packet_to_handle);
};
