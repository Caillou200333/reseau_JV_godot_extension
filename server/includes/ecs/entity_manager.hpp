#pragma once

#include "gameplay/common_tools.hpp"

#include <entt/entt.hpp>

#include <unordered_map>

struct ObjectContext {
    NetworkID network_id;
    ClassID class_id;
    entt::entity local_entity;
};

class EntityManager {
private:
    // NetworkID -> local entity ECS
    std::unordered_map<NetworkID, struct ObjectContext> network_to_entity;

    entt::registry _ecs;

    inline static const struct ObjectContext EmptyObject = {0, ClassID::PLAYER_CLASS, entt::null};

    NetworkID _next_network_id = 0;

    NetworkID GenerateNetworkID();

public:
    EntityManager() = default;
    
    const struct ObjectContext& CreateEntity(ClassID class_id);

    void RemoveEntity(NetworkID id);

    const struct ObjectContext& GetEntity(NetworkID id) const;

    bool HasEntity(NetworkID id) const;

    const std::unordered_map<NetworkID, struct ObjectContext>& AllEntities() const;
};
