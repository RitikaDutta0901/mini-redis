#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>

using namespace std;

class KeyValueStore {
private:
    unordered_map<string, string> store;
    mutable shared_mutex rw_mutex;
    ofstream wal;

public:
    explicit KeyValueStore(const string &wal_file = "mini_redis.wal");
    void set(const string &key, const string &value);
    bool get(const string &key, string &value) const;
    bool remove(const string &key);
};

#endif
