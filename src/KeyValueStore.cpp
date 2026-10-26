#include "KeyValueStore.h"
#include <sstream>

using namespace std;

KeyValueStore::KeyValueStore(const string &wal_file) {
    ifstream log(wal_file);
    string line;

    while (getline(log, line)) {
        istringstream input(line);
        string operation;
        getline(input, operation, '\t');

        if (operation == "SET") {
            string key, value;
            getline(input, key, '\t');
            getline(input, value);
            store[key] = value;
        } else if (operation == "DEL") {
            string key;
            getline(input, key, '\t');
            store.erase(key);
        }
    }

    log.close();
    wal.open(wal_file, ios::app);
}

void KeyValueStore::set(const string &key, const string &value) {
    unique_lock<shared_mutex> lock(rw_mutex);
    store[key] = value;
    wal << "SET\t" << key << '\t' << value << '\n';
    wal.flush();
}

bool KeyValueStore::get(const string &key, string &value) const {
    shared_lock<shared_mutex> lock(rw_mutex);
    auto it = store.find(key);
    if (it == store.end()) {
        return false;
    }
    value = it->second;
    return true;
}

bool KeyValueStore::remove(const string &key) {
    unique_lock<shared_mutex> lock(rw_mutex);
    bool removed = store.erase(key) > 0;

    if (removed) {
        wal << "DEL\t" << key << '\n';
        wal.flush();
    }

    return removed;
}
