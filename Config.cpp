//
// Created by LemanRass on 06.03.2022.
//

#include "Config.h"

string config_page;
vector<function<void()>> subscribers;
string configUrl;
ConfigData Config::localConfig;
bool isStarted;
thread configThread;

size_t write_data(char* page, size_t size, size_t nmemb, void* userdata) {

    int real_size = size * nmemb;

    for (int i = 0; i < real_size; i++) {
        config_page.push_back(page[i]);
    }

    return real_size;
}

ConfigData Config::DownloadNewConfig() {

    config_page.clear();

    CURL* curl;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, configUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 30000L);

    curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_global_cleanup();


    //Logger::Log(config_page);

    auto parsed = nlohmann::json::parse(config_page);

    string target = parsed["target_url"];
    bool started = parsed["is_started"];
    int timeout = parsed["timeout_ms"];
    int version = parsed["version"];

    return { target, started, timeout, version };
}

void Config::CheckConfigLoop() {

    ConfigData remoteConfig;

    while (isStarted) {

        try {
            remoteConfig = DownloadNewConfig();

            //If remote config has the newest version
            if (localConfig.version != remoteConfig.version) {
                localConfig = remoteConfig;
                Logger::Log("\nNew config version found!");
                Logger::Log(localConfig.toString());
                Emit();
            } else {
                Logger::Log("[ConfigUpdater] No updates found!");
            }

        } catch (const exception& ex) {
            Logger::Log("[ConfigUpdater] Failed to check config.");
            Logger::Log(ex.what());
        }

        this_thread::sleep_for(5s);
    }

    Logger::Log("[Config] Exit thread.");
}

void Config::Init(string &url) {

    configUrl = url;
    localConfig = DownloadNewConfig();
    Logger::Log(localConfig.toString());
}

void Config::Start() {

    isStarted = true;

    configThread = thread(CheckConfigLoop);
    configThread.detach();
}

void Config::Stop() {
    isStarted = false;
}

void Config::Subscribe(const function<void()> &f) {
    subscribers.push_back(f);
}

void Config::Emit() {

    for (auto &subscriber : subscribers) {
        subscriber();
    }
}


