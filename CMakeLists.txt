cmake_minimum_required(VERSION 3.21)
project(ddos)

set(CMAKE_CXX_STANDARD 14)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(ddos main.cpp Logger.cpp Logger.h Config.cpp Config.h Bot.cpp Bot.h ProxyList.cpp ProxyList.h)
target_link_libraries(ddos curl nlohmann_json::nlohmann_json)

