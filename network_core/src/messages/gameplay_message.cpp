#include "messages/gameplay_message.hpp"
#include "serializer.hpp"

GameplayMessage::GameplayMessage():BaseMessage(MessageType::GAMEPLAY) {}

GameplayMessage::GameplayMessage(const SnapShot& s)
    :BaseMessage(MessageType::GAMEPLAY),
    snapshot(s) {}


void GameplayMessage::SerializePayload() const {
    Serializer::Write(snapshot.frame_id);

    for (const struct EntityShot& entity_shot : snapshot.entities) {
        Serializer::Write(entity_shot.network_id);
        Serializer::Write(entity_shot.class_id);
        Serializer::Write(entity_shot.x);
        Serializer::Write(entity_shot.y);
    }
}

void GameplayMessage::DeserializePayload() {
    Serializer::Read(snapshot.frame_id);

    struct EntityShot entity_shot;
    MessageSize nb_entities = (_payload_size - sizeof(FrameID)) / (sizeof(NetworkID) + sizeof(ClassID) + sizeof(entity_shot.x) + sizeof(entity_shot.y)); 
    for (MessageSize i = 0; i < nb_entities; ++ i) {
        Serializer::Read(entity_shot.network_id);
        Serializer::Read(entity_shot.class_id);
        Serializer::Read(entity_shot.x);
        Serializer::Read(entity_shot.y);
        snapshot.AddEntityShot(entity_shot);
    }
}