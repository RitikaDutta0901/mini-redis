#ifndef KEYVALUESTORE_H
#define KEYVALUESTORE_H

#include <fstream>
#include <list>
#include <string>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>

using namespace std;

class KeyValueStore {
private:
    struct Entry {
        string value;
        list<string>::iterator position;
    };

    unordered_map<string, Entry> store;
    mutable list<string> lru;
    size_t capacity;
    mutable shared_mutex rw_mutex;
    ofstream wal;

public:
    explicit KeyValueStore(size_t max_capacity = 100,
                           const string &wal_file = "mini_redis.wal");
    void set(const string &key, const string &value);
    bool get(const string &key, string &value) const;
    bool remove(const string &key);
};

#endif
