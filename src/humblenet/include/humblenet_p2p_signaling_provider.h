#pragma once

#include "humblenet.h"

class ISignalingProvider {
    public:
        // State
        virtual ha_bool is_connected() = 0;

        // Commands
        virtual ha_bool connect(void* info) = 0;
        virtual void disconnect() = 0;

        virtual int send(const uint8_t* buff, size_t length) = 0;
        virtual int receive(const uint8_t* buff, size_t length, void* info) = 0;
};
