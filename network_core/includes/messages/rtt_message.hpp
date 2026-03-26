#pragma once

#include "messages/base_message.hpp"

class RTTMessage : public BaseMessage {
public:
    // Used for requests (just empty, must be initialized with Init before sending)
    RTTMessage();
    void Init();
    RTTMessage(uint32_t _id, uint64_t _t0, uint64_t _t1);

    // Used for responses
    RTTMessage(uint32_t _id, uint64_t _t0);
    RTTMessage(const RTTMessage& msg);
    RTTMessage& operator=(const RTTMessage& msg);

    static uint64_t NowMs();

    uint64_t CalculateRTT();

    uint32_t id;
    uint64_t t0 = 0; // client
    uint64_t t1 = 0; // server

private:
    inline static uint32_t _next_id = 0;

protected:
    virtual void SerializePayload() const override;
    virtual void DeserializePayload() override;
};