#pragma once 

#include "BaseMessage.tpp"

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

enum TypeMessage {
    HELO,
    HSK,
    MSG,
    BYE
}

class GreetingMessage : public BaseMessage<GreetingMessage> {
    GDCLASS(GreetingMessage, Object);

    TypeMessage _type;

    long unsigned int _size;

protected:
    static void _bind_methods();

    BaseMessage(TypeMessage type, long unsigned int size):_type(type),_size(size) {}

    BaseMessage(TypeMessage type):BaseMessage<Message>(type, 0) {}

public:
    TypeMessage Type(void) { return _type; }

    long unsigned int Size(void) { return _size; }

    uint8* Serialize(void);

    static Deserialize(uint8* incomming_data);
};