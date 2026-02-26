#pragma once

#include <queue>
#include <netinet/in.h>

struct Packet {
    uint8_t* data;
    unsigned int size;
    struct sockaddr_in sender_addr; // Client address of the sender of the packet

    Packet();
    Packet(const uint8_t* data_, unsigned int size_, const struct sockaddr_in& addr);
    Packet(const struct Packet& packet);
    struct Packet& operator=(const struct Packet& packet);

    ~Packet();
};

/**
 * @class PacketManager
 * @brief Store the received packet and make them accessicle to treatment later.
 */
class PacketManager {
public:
    PacketManager() = default;
    ~PacketManager();

    /**
     * @brief Add a received packet to the queue.
     * @param data buffer received
     * @param size buffer's size
     * @param addr sender's address
     */
    void PushPacket(struct Packet& packet);

    /**
     * @brief Return the next packet to be treated.
     * @return false if no packet is available, true else.
     */
    bool PopPacket(struct Packet& packet);

    /**
     * @brief Assert if the queue is empty.
     */
    bool IsEmpty() const;

    /**
     * @brief Empty the queue.
     */
    void Empty();

    /**
     * @brief Return the number of packets stored.
     */
    unsigned int Size() const;

private:
    std::queue<struct Packet> _queue;
};
