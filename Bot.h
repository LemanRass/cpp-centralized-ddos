//
// Created by LemanRass on 06.03.2022.
//

#ifndef DDOS_BOT_H
#define DDOS_BOT_H

#include <thread>
#include <vector>
#include <chrono>
#include <curl/curl.h>
#include "Config.h"
#include "Logger.h"

using namespace std;
using namespace std::chrono;

struct Request
{
    CURL* curl;
    long code;
};

class Bot {

private:

    static void threadLoop(int id);
    static size_t write_data(char *buffer, size_t size, size_t nmemb, void *userp);
    static Request createRequest();
    static void performRequest(Request &request);
    static void disposeRequest(Request request);

public:

    static void start();
    static void stop();
    static void pause();
    static void resume();
};


#endif //DDOS_BOT_H
