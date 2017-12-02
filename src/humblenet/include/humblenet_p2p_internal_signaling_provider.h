#pragma once

#include "humblepeer.h"
#include "libsocket.h"
#include "humblenet_p2p_signaling_provider.h"

namespace humblenet {

    class HumblenetSignalProvider : public ISignalingProvider {
        protected:
            internal_socket_t *wsi;
            std::vector<uint8_t> recvBuf;
            std::vector<char> sendBuf;

        public:
            HumblenetSignalProvider() : wsi(NULL) {}

            void test() {

            }

            // Status
            virtual ha_bool is_connected();

            // Commands
            virtual ha_bool connect(void* info);
            virtual void disconnect();

            virtual int send(const uint8_t* buff, size_t length);
            virtual int receive(const uint8_t* buff, size_t length, void* info);

            // Handlers
            static ha_bool p2pSignalProcess(const humblenet::HumblePeer::Message *msg, void *user_data);
    };

}
