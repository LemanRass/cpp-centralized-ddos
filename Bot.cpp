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
size_t Bot::write_data(char *buffer, size_t size, size_t nmemb, void *userp)
{
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

        //Check time
        auto nowTimestamp = high_resolution_clock::now();
        auto diff = duration_cast<milliseconds>(nowTimestamp - startTimestamp);


        //When 1 second passed collect statistic
        if (diff.count() > 1000) {

            char msg[42];
            sprintf(msg, "[Thread %d] Success: %d Failed: %d Total: %d", id, rps_success, rps_fail, rps_total);

            Logger::Log(msg);

            startTimestamp = high_resolution_clock::now();
            rps_total = 0;
            rps_success = 0;
            rps_fail = 0;
        }

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

    char output[22];
    sprintf(output, "Thread %d is finished!", id);
    Logger::Log(output);
}


void Bot::stop() {
    Logger::Log("Bot->Stop");
    isRunning = false;
    threads.clear();
}


void Bot::start() {

    Logger::Log("Bot->Start");
    isRunning = true;

    for (int i = 0; i < Config::localConfig.threads_count; i++) {
        threads.emplace_back(thread(threadLoop, i));
    }

    Logger::Log("");
    Logger::Log("Bot has been started! Threads: " + to_string(threads.size()));
    Logger::Log("");

    for (auto &thread : threads) {
        thread.detach();
    }
}

