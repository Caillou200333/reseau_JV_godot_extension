#include "messages/base_message.hpp"
#include "serializer.hpp"

#include <cstring>

BaseMessage::BaseMessage(MessageType type):_type(type) {}

MessageType BaseMessage::GetType() const { return _type; }

MessageSize BaseMessage::Serialize(uint8_t* data, unsigned int capacity) const {
    Serializer::SetWritingBuffer(data, capacity);

    MessageSize payload_size = 0;
    Serializer::Write(_type);
    Serializer::Write(payload_size); // tmp value as it is still unknown at this step 

    // Payload serialization
    SerializePayload();

    // Correct the size with the correct value
    unsigned int size = Serializer::GetWritingSize(); // final size of the message
    Serializer::SetWritingBuffer(data + sizeof(_type), sizeof(payload_size));
    payload_size = size - (sizeof(_type) + sizeof(payload_size));
    Serializer::Write(payload_size);

    return size;
}

void BaseMessage::Deserialize(const uint8_t* data, unsigned int size) {
    Serializer::SetReadingBuffer(data, size);

    MessageType type;
    Serializer::Read(type);
    if (_type != type)
        throw std::runtime_error("Invalid message type read");

    Serializer::Read(_payload_size);
    if (_payload_size + sizeof(_type) + sizeof(_payload_size) != size)
        throw std::runtime_error("Invalid message size read");

    // Payload deserialization
    DeserializePayload();
}

MessageType BaseMessage::PeekType(const uint8_t* data) {
    MessageType type;
    Serializer::SetReadingBuffer(data, sizeof(type));
    Serializer::Read(type);
    return type;
}