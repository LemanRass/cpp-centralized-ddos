#include <iostream>
#include <vector>
#include "Config.h"
#include "Bot.h"
#include "ProxyList.h"

using namespace std;
using namespace std::chrono;

void onConfigChanged() {

    Bot::stop();

    if (Config::localConfig.is_started) {
        Bot::start();
    }
}

int main() {

    ProxyList::Init();

    string configUrl;

    cout << "Enter config url or leave empty for default: " << endl;
    getline(cin, configUrl);

    if (configUrl.empty()) {
        configUrl = "https://pastebin.com/raw/yHpxtU1U";
    }

    Config::Subscribe(onConfigChanged);
    Config::Init(configUrl);

    Logger::Log(Config::localConfig.toString());
    Bot::start();

    Config::RunUpdater();

    return 0;
}
