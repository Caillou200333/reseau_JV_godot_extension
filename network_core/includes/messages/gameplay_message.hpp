#pragma once

#include "messages/base_message.hpp"
#include "gameplay/common_tools.hpp"

class GameplayMessage : public BaseMessage {
public:
    GameplayMessage();
    GameplayMessage(NetworkID network_id, ClassID class_id, uint32_t x, uint32_t y);

    NetworkID _network_id;
    ClassID _class_id;

    // Initial player position
    uint32_t _x;
    uint32_t _y;

protected:
    virtual void SerializePayload() const override;
    virtual void DeserializePayload() override;
};