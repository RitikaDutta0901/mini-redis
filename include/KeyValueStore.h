#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>

using namespace std;

class KeyValueStore {
private:
    unordered_map<string, string> store;
    mutable shared_mutex rw_mutex;

public:
    void set(const string &key, const string &value);
    bool get(const string &key, string &value) const;
    bool remove(const string &key);
};

#endif
