#ifndef HUMBLENET_SIGNALING
#define HUMBLENET_SIGNALING

#include "humblepeer.h"
#include "libsocket.h"
#include "humblenet_p2p_internal_signaling_provider.h"
#include <vector>

namespace humblenet {

    void register_protocol( internal_context_t* contet );

    // ha_bool p2pSignalProcess(const humblenet::HumblePeer::Message *msg, void *user_data);

}

ha_bool humblenet_signaling_connect();

#endif // HUMBLENET_SIGNALING
