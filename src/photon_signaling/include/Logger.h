
#pragma once

#include "Common-cpp/inc/Common.h"
#include <string>

namespace Photon {


    class Logger
    {
    public:
        Logger() {};
        ~Logger() {};

        void log(const ExitGames::Common::JString str);
    };
}
