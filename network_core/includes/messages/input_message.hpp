#pragma once

#include "messages/base_message.hpp"

#include <deque>
#include <unordered_map>

struct Input {
    uint32_t keys = InputID::INPUT_NONE;
    float aim_x = 0;
    float aim_y = 0;

    void AddKey(InputID key);
    bool HasKey(InputID key) const;
};

struct InputBuffer {
    // Store the historic of the last inputs sent/received (max = MAX_INPUTS)
    std::deque<struct Input> inputs;
    uint32_t next_sequence_to_get = 0;
    uint32_t next_sequence = 0;

    // Add an input to the buffer
    void AddInput(const struct Input& input);
    // Get the next input in file to get, if doesn't exist return nullptr
    const Input* GetNextInput();

private:
    // Remove an input from the buffer
    void RemoveInput(unsigned int nb_to_remove = 1);

    inline static const unsigned int MAX_INPUTS = 20;
};

class InputMessage : public BaseMessage {
public:
    InputMessage();

private:
    // Current player id set to deserialize (can be used for certain deserialization)
    inline static PlayerID _current_player_id = INVALID_ID;

    // Buffer of inputs to be sent (buffer of inputs serialized and sent)
    inline static struct InputBuffer _inputs_to_send;

    // Buffer of inputs received from other machines (buffer used to deserialize the received inputs from other players/server)
    inline static std::unordered_map<PlayerID, struct InputBuffer> _inputs_received;

    // Get the input buffer corresponding to given PlayerID, return nullptr if doesn't exist
    static struct InputBuffer* GetInputBuffer(PlayerID id);

public:
    // Set the current player id to deserialize the inputs (must be called before deserialization of an input message)
    static void SetPlayerID(PlayerID new_player_id);

    // Add an input to be sent
    static void AddInput(const struct Input& input);

    // Get the next input in file to get from buffer corresponding to PlayerID, if doesn't exist return nullptr
    static const Input* GetNextInput(PlayerID player_id);

protected:
    virtual void SerializePayload() const override;
    virtual void DeserializePayload() override;
};