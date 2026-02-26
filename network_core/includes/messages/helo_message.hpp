#pragma once

#include "messages/base_message.hpp"

class HELOMessage : public BaseMessage {
public:
    HELOMessage();

protected:
    virtual void SerializePayload() const override;
    virtual void DeserializePayload() override;
};