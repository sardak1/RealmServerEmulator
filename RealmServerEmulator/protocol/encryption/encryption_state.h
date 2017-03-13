// Copyright 2017 Sardak Software, All Rights Reserved

#ifndef REALMSERVEREMULATOR_PROTOCOL_ENCRYPTION_ENCRYPTION_STATE_H_
#define REALMSERVEREMULATOR_PROTOCOL_ENCRYPTION_ENCRYPTION_STATE_H_

#include <cstdint>

namespace protocol
{

    struct EncryptionState final
    {
        struct SubState
        {
            uint32_t key;
            uint32_t multiplier;
        } encryption, decryption;
    };

    uint32_t create_random_bytes(int count);
    EncryptionState create_encryption_state();

}  // namespace protocol

#endif  // REALMSERVEREMULATOR_PROTOCOL_ENCRYPTION_ENCRYPTION_STATE_H_
