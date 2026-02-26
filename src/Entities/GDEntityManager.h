#pragma once

#include "Entities/GDEntity.h"
#include "gameplay/common_tools.hpp"

#include <godot_cpp/classes/node.hpp>

#include <unordered_map>
#include <cstdint>

using EntityFactoryFunction = GDEntity*(*)(void);
using GDClassID = uint32_t;

class GDEntityManager : public Node {
    GDCLASS(GDEntityManager, Node)

protected:
	static void _bind_methods();

private:
    // ClassID -> entity factory ECS
    std::unordered_map<ClassID, EntityFactoryFunction> _entity_factory;

    // NetworkID -> local entity ECS
    std::unordered_map<NetworkID, GDEntity*> _network_to_entity;

public:
    GDEntityManager();
    ~GDEntityManager();

    GDEntity* CreateEntity(NetworkID network_id, GDClassID class_id);

    void RemoveEntity(NetworkID id);

    GDEntity* GetEntity(NetworkID id) const;

    bool HasEntity(NetworkID id) const;
};