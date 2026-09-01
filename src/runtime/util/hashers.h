#pragma once

#include <xxhash.h>
#include "core/types.h"

namespace gr
{

// Use this random seed for hashes that should persist across runtimes
// e.g. pipeline state hashes for the pso library
// Note: just randomly used some hexvaluesfor this seed
constexpr U64 PERSISTENT_HASH_SEED_64 = 0xfb6721dc10df25f3;
constexpr U32 PERSISTENT_HASH_SEED_32 = 0x10df25f3;

inline U64 XXH64_Create(void* buffer, U64 size, U64 seed = PERSISTENT_HASH_SEED_64)
{
    return XXH64(buffer, size, seed);
}

inline U32 XXH32_Create(void* buffer, U64 size, U32 seed = PERSISTENT_HASH_SEED_32)
{
    return XXH32(buffer, size, seed);
}

}
