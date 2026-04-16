#include "messages/text_message.hpp"
#include "serializer.hpp"

TextMessage::TextMessage():TextMessage(nullptr, 0) {}

TextMessage::TextMessage(const char* data, MessageSize size):BaseMessage(MessageType::TEXT) {
    _payload_size = size;
    if (_payload_size <= 0) {
        _data = nullptr;
        return;
    }
    
    ++ _payload_size;
    _data = new char[_payload_size];
    std::memcpy(_data, data, _payload_size);
    _data[_payload_size - 1] = 0;
}

TextMessage::TextMessage(const TextMessage& msg):TextMessage(msg._data, msg._payload_size) {}

TextMessage& TextMessage::operator=(const TextMessage& msg) {
    if (this == &msg)
        return *this;

    if (_data) delete[] _data;

    _payload_size = msg._payload_size;
    
    if (_payload_size <= 0) {
        _data = nullptr;
        return *this;
    }
    
    _data = new char[_payload_size];
    std::memcpy(_data, msg._data, _payload_size);

    return *this;
}

TextMessage::~TextMessage() { if (_data) delete[] _data; }

const char* TextMessage::GetText() const { return _data; }

void TextMessage::SerializePayload() const {
    Serializer::WriteBytes(_data, _payload_size);
}

void TextMessage::DeserializePayload() {
    if (!_data) {
        _data = new char[_payload_size];
    }

    Serializer::ReadBytes(_data, _payload_size);
}