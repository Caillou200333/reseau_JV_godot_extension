#pragma once

#include "gameplay/common_tools.hpp"

#include <entt/entt.hpp>

#include <unordered_map>

struct Position {
    double x;
    double y;
};

struct Velocity : Position {};

struct ObjectContext {
    NetworkID network_id;
    ClassID class_id;
    entt::entity local_entity;

    struct Position& GetPosition() const;
    struct Velocity& GetVelocity() const;

    inline static entt::registry _ecs;
};

class EntityManager {
private:
    // NetworkID -> local entity ECS
    std::unordered_map<NetworkID, struct ObjectContext> network_to_entity;

    inline static struct ObjectContext* EmptyObject = nullptr;

    NetworkID _next_network_id = 0;

    NetworkID GenerateNetworkID();

public:
    EntityManager() = default;
    
    struct ObjectContext* CreateEntity(ClassID class_id, double x = 0., double y = 0.);

    void RemoveEntity(NetworkID id);

    struct ObjectContext* GetEntity(NetworkID id);

    bool HasEntity(NetworkID id) const;

    const std::unordered_map<NetworkID, struct ObjectContext>& AllEntities() const;
};
