#pragma once

#include <cstdint>

// ID specific to a player's connection
using PlayerID = uint32_t;

#define SERVER_ID ((PlayerID) 0)
#define INVALID_ID ((PlayerID) -1)

// ID specific to an entity present in the world
using NetworkID = uint32_t;

// ID specific to a class of entity present in the game
enum ClassID : uint32_t {
    PLAYER_CLASS,
};

// ID reprensenting the kind of inputs 
enum InputID : uint32_t {
    INPUT_NONE   = 0,

    // Movement
    INPUT_UP     = 1 << 0,
    INPUT_DOWN   = 1 << 1,
    INPUT_LEFT   = 1 << 2,
    INPUT_RIGHT  = 1 << 3,

    // Action
    INPUT_JUMP   = 1 << 4,
    INPUT_SPRINT = 1 << 6,
};