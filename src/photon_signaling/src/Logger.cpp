/*
* @Author: ramonmelo
* @Date:   2017-11-09
* @Last Modified by:   ramonmelo
* @Last Modified time: 2017-11-10
*/

#include "Logger.h"

#include <iostream>

void Logger::log(const ExitGames::Common::JString str) {
    std::wcout << "LOG: " << str.cstr() << std::endl;
}
