#include "Entities/GDEntityManager.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void GDEntityManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("create", "network_id", "class_id"), &GDEntityManager::CreateEntity);
    ClassDB::bind_method(D_METHOD("remove", "network_id"), &GDEntityManager::RemoveEntity);
    ClassDB::bind_method(D_METHOD("get", "network_id"), &GDEntityManager::GetEntity);
    ClassDB::bind_method(D_METHOD("has", "network_id"), &GDEntityManager::HasEntity);
}

GDEntityManager::GDEntityManager() {
    // Registry the lambda factory functions to create entities
    _entity_factory[ClassID::PLAYER_CLASS] = [](void){ return memnew(GDEntity); };
}

GDEntityManager::~GDEntityManager() {
    for (auto& pair : _network_to_entity) {
        GDEntity* entity = pair.second;

        if (entity && !entity->is_queued_for_deletion()) {
            entity->queue_free();
        }
    }

    _network_to_entity.clear();
    _entity_factory.clear();
}

GDEntity* GDEntityManager::CreateEntity(NetworkID network_id, GDClassID class_id) {
    ClassID _class_id = static_cast<ClassID>(class_id);
    if (HasEntity(network_id)) {
        UtilityFunctions::printerr("GDEntityManager: An entity is already registered under NetworkID ", network_id, " creation failed.");
        return _network_to_entity[network_id];
    }

    auto it = _entity_factory.find(_class_id);
    if (it == _entity_factory.end()) {
        UtilityFunctions::printerr("GDEntityManager: No factory registered for this ClassID.");
        return nullptr;
    }

    EntityFactoryFunction factory = it->second;
    if (!factory) {
        UtilityFunctions::printerr("GDEntityManager: Factory function is null.");
        return nullptr;
    }

    GDEntity* entity = factory();
    if (!entity) {
        UtilityFunctions::printerr("GDEntityManager: Factory failed to create entity.");
        return nullptr;
    }

    // cast du pointeur en uint64_t
    uint64_t ptr_val = reinterpret_cast<uint64_t>(entity);

    // afficher en hexadécimal
    UtilityFunctions::print("GDEntityManager: entity created at 0x" + String::num_uint64(ptr_val, 16) + ".");
    add_child(entity);
    entity->set_owner(get_owner());
    _network_to_entity[network_id] = entity;

    return entity;
}

void GDEntityManager::RemoveEntity(NetworkID id) {
    auto it = _network_to_entity.find(id);
    if (it == _network_to_entity.end()) {
        return;
    }

    GDEntity* entity = it->second;
    if (entity) {
        // Godot-safe deletion
        entity->queue_free();
    }

    _network_to_entity.erase(it);
}

GDEntity* GDEntityManager::GetEntity(NetworkID id) const {
    auto it = _network_to_entity.find(id);
    if (it != _network_to_entity.end()) return it->second;
    return nullptr;
}

bool GDEntityManager::HasEntity(NetworkID id) const {
    return _network_to_entity.find(id) != _network_to_entity.end();
}
