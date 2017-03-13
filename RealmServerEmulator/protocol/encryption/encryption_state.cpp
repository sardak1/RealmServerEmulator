// Copyright 2017 Sardak Software

#include <random>
#include "protocol/encryption/encryption_state.h"

namespace
{

    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> distribution(0, 255);

    void init_substate(protocol::EncryptionState::SubState* substate)
    {
        //substate->key = protocol::create_random_bytes(4);
        //substate->multiplier = 0x10000 | protocol::create_random_bytes(2);
        substate->key = 0;
        substate->multiplier = 1;
    }

}  // namespace

uint32_t protocol::create_random_bytes(int count)
{
    uint32_t result = 0;

    while (count-- > 0)
    {
        result |= (distribution(generator) << (3 << count));
    }

    return result;
}

protocol::EncryptionState protocol::create_encryption_state()
{
    EncryptionState state;

    init_substate(&state.decryption);
    init_substate(&state.encryption);

    return state;
}
