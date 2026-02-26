#pragma once

#include "messages/base_message.hpp"
#include "gameplay/common_tools.hpp"

class GameplayMessage : public BaseMessage {
public:
    GameplayMessage();
    GameplayMessage(NetworkID network_id, ClassID class_id, uint8_t x, uint8_t y);

    NetworkID _network_id;
    ClassID _class_id;

    // Initial player position
    uint8_t _x;
    uint8_t _y;

protected:
    virtual void SerializePayload() const override;
    virtual void DeserializePayload() override;
};