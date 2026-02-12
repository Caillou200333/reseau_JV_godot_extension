#pragma once 

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <unordered_map>
#include <cstdint> 

using namespace godot;

using namespace std;

enum MessageType;

class BaseMessage : public Object {
    GDCLASS(BaseMessage, Object);

    using CreateMsgType = function<BaseMessage()>;

    static inline unordered_map<MessageType, CreateMsgType> _msg_factory;

    MessageType _type;
    
    BaseMessage() = 0;

private:
    static MessageType PeekType(const uint8_t* data_received) {}

    static BaseMessage Create(MessageType type) {
        if (messageRegistry.find(type) != messageRegistry.end()) {
            return _msg_factory.at(type)();
        }
        return BaseMessage(-1);
    }

    virtual void Deserialize(uint8_t* data_received) = 0;

protected:
    static void _bind_methods();

    BaseMessage(MessageType type):_type(type) {}

public:
    template<typename T>
    static void Register() {
        if (_msg_factory.find(T::Type) == _msg_factory.end())
            _msg_factory.emplace(T::Type, [](void){ return T(); });
    }
    /*
    template<typename T>
    bool Is() const {
        static_assert(std::is_base_of_v<BaseMessage, T>);
        return type == T::Type;
    }*/

    template<typename T>
    T& Get() {
        Expects(Is<T>());
        return static_cast<T&>(*this);
    }

    template<typename T>
    const T& Get() const {
        Expects(Is<T>());
        return static_cast<const T&>(*this);
    }

    virtual void Serialize(uint8_t** data_to_send) const = 0;

    static BaseMessage Deserialize(const uint8_t* data_received) {}
};

template<typename Payload derived_from Message>
class Message : public BaseMessage {

protected:
    static MessageType Type;

private:
    long unsigned int _size;

    long unsigned int _id; // Static ?

protected:
    Message():Message(0, 0) {}

    Message(long unsigned int size, long unsigned int id):BaseMessage(Type),_size(size),_id(id) {}

    void SetSize(long unsigned int new_size) { _size = new_size; }

    void SetId(long unsigned int new_id) { _id = new_id; }

public:
    virtual void Serialize(uint8_t** data_to_send) const override {}

    virtual void Deserialize(const uint8_t* data_received) override {}

    static TypeMessage Type(void) { return Type; }

    long unsigned int Size(void) { return _size; }

    long unsigned int Id(void) { return _id; }
};