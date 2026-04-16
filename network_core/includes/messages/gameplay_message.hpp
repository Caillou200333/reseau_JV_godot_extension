#pragma once

#include "messages/base_message.hpp"
#include "gameplay/snapshot.hpp"

class GameplayMessage : public BaseMessage {
public:
    GameplayMessage();
    GameplayMessage(const SnapShot& s);

    SnapShot snapshot;

protected:
    virtual void SerializePayload() const override;
    virtual void DeserializePayload() override;
};