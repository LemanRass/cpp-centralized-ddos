//
// Created by LemanRass on 06.03.2022.
//

#ifndef DDOS_CONFIG_H
#define DDOS_CONFIG_H

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include "Logger.h"

using namespace std;

struct ConfigData {
    string target_url;
    int threads_count;
    bool is_started;
    int timeout_ms;
    int version;

    string toString() {
        return "Target: " + target_url + "\n" +
            "Threads: " + to_string(threads_count) + "\n" +
            "IsStarted: " + to_string(is_started) + "\n" +
            "Timeout: " + to_string(timeout_ms) + "\n" +
            "Version: " + to_string(version) + "\n";
    }
};

class Config {

private:
    static void Emit();
    static ConfigData DownloadNewConfig();
    static void CheckConfigLoop();


public:
    static ConfigData localConfig;

    static void Init(string &url);
    static void Subscribe(const function<void()>&);
    static void Start();
    static void Stop();
};



#endif //DDOS_CONFIG_H
