// Copyright 2017 Sardak Software

#include "protocol/encryption/packet_encryption.h"

namespace
{

    void advance_key(protocol::EncryptionState::SubState* substate)
    {
        substate->key = (substate->key * substate->multiplier) + 1;
    }

}  // namespace

bool protocol::decrypt_packet(protocol::EncryptionState* state, void* packet)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(packet);
    auto& substate = state->decryption;
    auto word_count = words[0] >> 2;
    uint32_t checksum = 0;

    for (uint32_t index = 1; index <= word_count; index++)
    {
        advance_key(&substate);
        words[index] ^= substate.key;
        checksum ^= words[index];
    }

    return checksum == words[word_count + 1];
}

void protocol::encrypt_packet(protocol::EncryptionState* state, void* packet)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(packet);
    auto& substate = state->encryption;
    auto word_count = words[0] >> 2;
    uint32_t checksum = 0;

    for (uint32_t index = 1; index <= word_count; index++)
    {
        if (index < 5)
        {
            advance_key(&substate);
            words[index] ^= substate.key;
        }

        checksum ^= words[index];
    }

    words[word_count + 1] = checksum;
}
