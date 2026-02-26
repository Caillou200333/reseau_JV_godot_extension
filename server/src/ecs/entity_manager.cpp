#include "ecs/entity_manager.hpp"


NetworkID EntityManager::GenerateNetworkID() {
    return _next_network_id ++;
}

const struct ObjectContext& EntityManager::CreateEntity(ClassID class_id) {
    entt::entity entity = _ecs.create();
    NetworkID network_id = GenerateNetworkID();
    network_to_entity[network_id] = {network_id, class_id, entity};
    return GetEntity(network_id);
}

void EntityManager::RemoveEntity(NetworkID id) {
    network_to_entity.erase(id);
}

const struct ObjectContext& EntityManager::GetEntity(NetworkID id) const {
    auto it = network_to_entity.find(id);
    if (it != network_to_entity.end()) return it->second;
    return EntityManager::EmptyObject;
}

bool EntityManager::HasEntity(NetworkID id) const {
    return network_to_entity.find(id) != network_to_entity.end();
}

const std::unordered_map<NetworkID, struct ObjectContext>& EntityManager::AllEntities() const {
    return network_to_entity;
}
