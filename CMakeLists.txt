# CMakeList.txt : CMake project for cpp-centralized-ddos, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("cpp-centralized-ddos")

# Add source to this project's executable.
add_executable (cpp-centralized-ddos 
"cpp-centralized-ddos.cpp" "cpp-centralized-ddos.h"
"Logger.cpp" "Logger.h"
"Config.cpp" "Config.h"
"Bot.cpp" "Bot.h"
"ProxyList.cpp" "ProxyList.h")

# TODO: Add tests and install targets if needed.
find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(cpp-centralized-ddos PRIVATE CURL::libcurl)
target_link_libraries(cpp-centralized-ddos PRIVATE nlohmann_json::nlohmann_json)
