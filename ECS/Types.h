#pragma once

#include <bitset>
#include <cstdint>


// ECS
using Entity = std::int32_t;
const Entity NULL_ENTITY = -1;
const Entity MAX_ENTITIES = 500;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

