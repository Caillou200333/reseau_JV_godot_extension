#pragma once 

#include <cstdint>

enum MessageType : uint8_t {
    INVALID,
    HELO,
    HANDSHAKE,
    GAMEPLAY,
    BYE,
    TEXT
};

typedef unsigned int MessageSize;

/**
 * @class Parent class for all kind of messsages sent.
 */
class BaseMessage {
protected:
    BaseMessage() = delete;
    BaseMessage(MessageType type);

    MessageSize _payload_size = 0;

    /**
     * @brief Method to implement in order to de/serialize the payload of the message.
     */
    virtual void SerializePayload() const = 0;
    virtual void DeserializePayload() = 0;

public:
    virtual ~BaseMessage() = default;

    /**
     * @brief Return the type of the message.
     */
    MessageType GetType() const;

    /**
     * @brief Serialize the message into the given 'data' parameter.
     * @param data : must have enough capacity to contain the message and non nullptr.
     * @return The final size of the serialized message inside 'data'.
     */
    MessageSize Serialize(uint8_t* data, unsigned int capacity) const;

    /**
     * @brief Desrialize the message from the given 'data' parameter.
     * @param data : must be at least longer than size and non nullptr.
     */
    void Deserialize(const uint8_t* data, unsigned int size);

    /**
     * @brief Peek at the first part of the serialized message to determine the type of the message to create.
     */
    static MessageType PeekType(const uint8_t* data);

private: 
    // Header for all messages sent/received
    MessageType _type;
};