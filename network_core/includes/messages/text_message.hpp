#pragma once

#include "messages/base_message.hpp"

class TextMessage : public BaseMessage {
public:
    TextMessage();
    TextMessage(const char* data, MessageSize size);
    TextMessage(const TextMessage& msg);
    TextMessage& operator=(const TextMessage& msg);

    ~TextMessage();

    const char* GetText() const;

protected:
    virtual void SerializePayload() const override;
    virtual void DeserializePayload() override;

private:
    char* _data;
};