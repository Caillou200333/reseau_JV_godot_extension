#include "Network/Messages/GDGameplayMessage.h"
#include "messages/gameplay_message.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;


#include <godot_cpp/variant/utility_functions.hpp>

void GDGameplayMessage::_bind_methods() {
    ClassDB::bind_method(D_METHOD("save_snapshot", "manager"), &GDGameplayMessage::SaveSnapShot);
}

GDGameplayMessage::GDGameplayMessage() {
    _message = std::make_shared<GameplayMessage>();
}

void GDGameplayMessage::SaveSnapShot(Ref<GDSnapShotManager> manager) const {
    if (manager.is_null()) {
        return;
    }

    auto gmplMsg = std::static_pointer_cast<GameplayMessage>(_message);
    manager->manager.SaveSnapShot(gmplMsg->snapshot);
}