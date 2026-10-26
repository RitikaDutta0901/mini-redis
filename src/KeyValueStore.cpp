#include "KeyValueStore.h"
#include <sstream>

using namespace std;

KeyValueStore::KeyValueStore(size_t max_capacity, const string &wal_file)
    : capacity(max_capacity) {
    if (capacity == 0) {
        capacity = 1;
    }

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

            auto it = store.find(key);
            if (it != store.end()) {
                it->second.value = value;
                lru.splice(lru.begin(), lru, it->second.position);
            } else {
                if (store.size() >= capacity) {
                    string old_key = lru.back();
                    lru.pop_back();
                    store.erase(old_key);
                }
                lru.push_front(key);
                store[key] = Entry{value, lru.begin()};
            }
        } else if (operation == "DEL") {
            string key;
            getline(input, key, '\t');

            auto it = store.find(key);
            if (it != store.end()) {
                lru.erase(it->second.position);
                store.erase(it);
            }
        }
    }

    log.close();
    wal.open(wal_file, ios::app);
}

void KeyValueStore::set(const string &key, const string &value) {
    unique_lock<shared_mutex> lock(rw_mutex);

    auto it = store.find(key);
    if (it != store.end()) {
        it->second.value = value;
        lru.splice(lru.begin(), lru, it->second.position);
    } else {
        if (store.size() >= capacity) {
            string old_key = lru.back();
            lru.pop_back();
            store.erase(old_key);
            wal << "DEL\t" << old_key << '\n';
        }

        lru.push_front(key);
        store[key] = Entry{value, lru.begin()};
    }

    wal << "SET\t" << key << '\t' << value << '\n';
    wal.flush();
}

bool KeyValueStore::get(const string &key, string &value) const {
    unique_lock<shared_mutex> lock(rw_mutex);

    auto it = store.find(key);
    if (it == store.end()) {
        return false;
    }

    value = it->second.value;
    lru.splice(lru.begin(), lru, it->second.position);
    return true;
}

bool KeyValueStore::remove(const string &key) {
    unique_lock<shared_mutex> lock(rw_mutex);

    auto it = store.find(key);
    if (it == store.end()) {
        return false;
    }

    lru.erase(it->second.position);
    store.erase(it);
    wal << "DEL\t" << key << '\n';
    wal.flush();
    return true;
}
