//
// Created by LemanRass on 06.03.2022.
//

#include "Logger.h"
#include <mutex>

mutex m;

void Logger::Log(std::string msg) {

    lock_guard<mutex>lock(m);
    cout << msg << endl;
}


