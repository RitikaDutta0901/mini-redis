#include <iostream>
#include <string>
#include "KeyValueStore.h"

using namespace std;

int main() {
    KeyValueStore store;
    string command;

    cout << "Mini Redis - Phase 2" << endl;
    cout << "Commands: SET key value | GET key | DEL key | EXIT" << endl;

    while (true) {
        cout << "> ";
        if (!(cin >> command)) {
            break;
        }

        if (command == "EXIT") {
            break;
        } else if (command == "SET") {
            string key, value;
            cin >> key >> value;
            store.set(key, value);
            cout << "OK" << endl;
        } else if (command == "GET") {
            string key, value;
            cin >> key;
            if (store.get(key, value)) {
                cout << value << endl;
            } else {
                cout << "(nil)" << endl;
            }
        } else if (command == "DEL") {
            string key;
            cin >> key;
            if (store.remove(key)) {
                cout << "1" << endl;
            } else {
                cout << "0" << endl;
            }
        } else {
            cout << "Unknown command" << endl;
        }
    }

    return 0;
}
