#include "KeyValueStore.h"

using namespace std;

void KeyValueStore::set(const string &key, const string &value) {
    unique_lock<shared_mutex> lock(rw_mutex);
    store[key] = value;
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
    return store.erase(key) > 0;
}
