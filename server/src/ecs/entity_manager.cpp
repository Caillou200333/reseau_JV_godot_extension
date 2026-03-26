#include "ecs/entity_manager.hpp"

struct Position& ObjectContext::GetPosition() const {
    return ObjectContext::_ecs.get<Position>(local_entity);
}

struct Velocity& ObjectContext::GetVelocity() const {
    return ObjectContext::_ecs.get<Velocity>(local_entity);
}

NetworkID EntityManager::GenerateNetworkID() {
    return _next_network_id ++;
}

struct ObjectContext* EntityManager::CreateEntity(ClassID class_id, double x, double y) {
    entt::entity entity = ObjectContext::_ecs.create();
    ObjectContext::_ecs.emplace<Position>(entity, x, y);
    ObjectContext::_ecs.emplace<Velocity>(entity, 0., 0.);

    NetworkID network_id = GenerateNetworkID();
    network_to_entity[network_id] = {network_id, class_id, entity};
    return GetEntity(network_id);
}

void EntityManager::RemoveEntity(NetworkID id) {
    network_to_entity.erase(id);
}

struct ObjectContext* EntityManager::GetEntity(NetworkID id) {
    auto it = network_to_entity.find(id);
    if (it != network_to_entity.end()) return &it->second;
    return EntityManager::EmptyObject;
}

bool EntityManager::HasEntity(NetworkID id) const {
    return network_to_entity.find(id) != network_to_entity.end();
}

const std::unordered_map<NetworkID, struct ObjectContext>& EntityManager::AllEntities() const {
    return network_to_entity;
}
