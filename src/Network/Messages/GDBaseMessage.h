#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

#include "messages/base_message.hpp"

#include <memory>

class GDBaseMessage : public RefCounted {
    GDCLASS(GDBaseMessage, RefCounted);

protected:
    static void _bind_methods();

    std::shared_ptr<BaseMessage> _message;

public:
    GDBaseMessage() = default;
    ~GDBaseMessage() = default;

    std::shared_ptr<BaseMessage> GetMessage();
};
