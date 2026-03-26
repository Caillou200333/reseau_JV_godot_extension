#pragma once


////// NOT USED //// IN PROGRESS

enum GameplayType : uint8_t {
    INPUT,
    STATE,
    EVENT,
    SYNC
};

struct GameplayPayload {
protected:
    GameplayPayload() = delete;
    GameplayPayload(GameplayType type);

public:
    virtual ~GameplayPayload() = default;

    /**
     * @brief Serialize the message into the given 'data' parameter.
     * @param data : must have enough capacity to contain the message and non nullptr.
     * @return The final size of the serialized message inside 'data'.
     */
    MessageSize Serialize(uint8_t* data, unsigned int capacity) const;

    /**
     * @brief Desrialize the message from the given 'data' parameter.
     * @param data : must be at least longer than size and non nullptr.
     */
    void Deserialize(const uint8_t* data, unsigned int size);

    /**
     * @brief Peek at the first part of the serialized payload to determine the type of the payload to create.
     */
    static GameplayType PeekType(const uint8_t* data);

    /**
     * @brief Return the type of the payload.
     */
    GameplayType GetGameplayType() const;

private:
    GameplayType _gp_type;
};