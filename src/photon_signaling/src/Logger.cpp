/*
* @Author: ramonmelo
* @Date:   2017-11-09
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2017-11-28
*/

#include "Logger.h"

#include <iostream>

using namespace Photon;

void Logger::log(const ExitGames::Common::JString str) {
    std::wcout << "LOG: " << str.cstr() << std::endl;
}
