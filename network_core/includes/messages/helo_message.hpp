#pragma once

#include "messages/base_message.hpp"

class HELOMessage : public BaseMessage {
public:
    HELOMessage();
    HELOMessage(double f, double s, NetworkID e);

    double frame_length = (1. / 60.);
    double character_speed = 200.;
    NetworkID entity_controlled = 0;

protected:
    virtual void SerializePayload() const override;
    virtual void DeserializePayload() override;
};