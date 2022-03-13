//
// Created by LemanRass on 11.03.2022.
//

#include "ProxyList.h"

vector<string> proxies;
int currProxyNum = 0;

string ReadLineFromFile(FILE *f) {
    string buffer;
    int c;
    do {
        c = fgetc(f);
        if (c != EOF && c != '\n') {
            buffer.push_back((char)c);
        }
    } while (c != EOF && c != '\n');

    return buffer;
}

void ReadProxiesFromFile(const string &path) {


    FILE *f = fopen(path.c_str(), "rb+");

    if (f == nullptr) {
        cout << "Proxies not found! Skipping..." << endl;
        return;
    }


    string line;
    do {

        line = ReadLineFromFile(f);
        if (!line.empty()) {
            string ip = line.substr(0, line.find(':'));
            string port = line.substr(line.find(':') + 1, line.size() - line.find(':') + 1);
            proxies.push_back(ip + ":" + port);
        }

    } while (!line.empty());

    cout << "Loaded " << proxies.size() << " proxies." << endl;
}

void ProxyList::Init() {

    ReadProxiesFromFile("./proxies.txt");
}

string ProxyList::PickNext() {

    currProxyNum++;

    if (currProxyNum >= proxies.size()) {
        currProxyNum = 0;
    }

    return proxies[currProxyNum];
}
