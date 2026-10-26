#include "KeyValueStore.h"

using namespace std;

void KeyValueStore::set(const string &key, const string &value) {
    store[key] = value;
}

bool KeyValueStore::get(const string &key, string &value) const {
    auto it = store.find(key);
    if (it == store.end()) {
        return false;
    }
    value = it->second;
    return true;
}

bool KeyValueStore::remove(const string &key) {
    return store.erase(key) > 0;
}
