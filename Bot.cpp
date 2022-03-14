//
// Created by LemanRass on 06.03.2022.
//

#include "Bot.h"
#include "ProxyList.h"

auto startTimestamp = high_resolution_clock::now();
int rps_total, rps_success, rps_fail;
bool isRunning;
vector<thread> threads;

string page;
size_t Bot::write_data(char *buffer, size_t size, size_t nmemb, void *userp) {
    
    size_t realSize = size * nmemb;

    /*for (int i = 0; i < realSize; i++) {
        page.push_back(buffer[i]);
    }*/

    return realSize;
}

Request Bot::createRequest() {

    CURL* curl;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, Config::localConfig.target_url.c_str());
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

    if (ProxyList::GetProxiesCount() > 0) {
        curl_easy_setopt(curl, CURLOPT_PROXY, ProxyList::PickNext().c_str());
    }

    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, Config::localConfig.timeout_ms);

    return { curl };
}

void Bot::performRequest(Request &request) {

    curl_easy_perform(request.curl);
    curl_easy_getinfo(request.curl, CURLINFO_RESPONSE_CODE, &request.code);
}

void Bot::disposeRequest(Request request) {

    curl_easy_cleanup(request.curl);
}

void Bot::threadLoop(int id) {

    auto request = createRequest();

    while (isRunning) {

        //Send request
        performRequest(request);

        //Logger::Log(to_string(request.code));

        if (request.code == 200) {
            rps_success++;
        } else {
            rps_fail++;
        }

        rps_total++;
    }

    disposeRequest(request);
    curl_global_cleanup();

    Logger::Log("Thread " + to_string(id) + " is finished!");
}

void Bot::updateStats() {

    if (!isRunning)
        return;

    Logger::Log("[Stats] Success: " + to_string(rps_success) +
        " Fail: " + to_string(rps_fail) +
        " Total: " + to_string(rps_total));

    startTimestamp = high_resolution_clock::now();

    rps_total = 0;
    rps_success = 0;
    rps_fail = 0;

    this_thread::sleep_for(seconds(1));
    updateStats();
}

void Bot::stop() {

    Logger::Log("Bot->Stop");
    isRunning = false;
    threads.clear();
    rps_total = 0;
    rps_fail = 0;
    rps_success = 0;
    this_thread::sleep_for(seconds(5));
}


void Bot::start(int threadCount) {

    Logger::Log("Bot->Start");
    isRunning = true;

    for (int i = 0; i < threadCount; i++) {
        threads.emplace_back(thread(threadLoop, i));
    }

    Logger::Log("");
    Logger::Log("Bot has been started! Threads: " + to_string(threads.size()));
    Logger::Log("");

    for (auto &thread : threads) {
        thread.detach();
    }

    thread t1(updateStats);
    t1.detach();
}

