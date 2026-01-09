#pragma once
#include <stdexcept>

enum class state : char { // char because 3 states means I only need one byte, and too big for 1 bit, they should hurry up and produce q-bits
    Filled = 'F',
    Empty = 'E',
    Tombstone = 'T'
};

template <typename K, typename T>
struct slot{
    K key;
    T data;
    size_t hash;
    state state;
    slot(K k, T d, size_t h) : key{k}, data{d}, hash{h}, state{state::Filled} {}; // constructor automatically assigns as filled
    slot() : key{}, data{}, hash{}, state{state::Empty} {};                       // default should be empty for flat map
};

template <typename K, typename T>
class hash_map {
    using data_type = slot<K, T>;

    data_type* data;
    size_t capacity{128};                // default size, resizes at 75% full for amortized benefits etc, power of two to make use of bitmask
    size_t mask{capacity - 1};
    size_t s{0};
    void resize();
public:
    hash_map();                          // constructor
    ~hash_map();                         // destructor
    void insert(const K& key, const T& val);  // setter
    void erase(const K& key);
    const T& at(const K& key);             // getter
    bool contains(const K& key);
    T& operator[](const K& key);              // subscript operator
    size_t size() const { return s; }
    size_t cap() const { return capacity; }
};

template<typename K, typename T>
hash_map<K, T>::hash_map() {
    data = new data_type[capacity]{};
}

template<typename K, typename T>
hash_map<K, T>::~hash_map() {
    delete[] data;
}

template<typename K, typename T>
void hash_map<K, T>::insert(const K& key, const T& val) {
    size_t hash = std::hash<K>{}(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && !(data[h].hash == hash && data[h].key == key)) h = (h + 1) & mask;
    bool inserted = (data[h].state != state::Filled);
    data[h] = {key, val, hash};
    if (inserted && ++s > capacity * 3 / 5) resize(); // I have gone with smaller load percentage for open addressing
}

template<typename K, typename T>
void hash_map<K, T>::erase(const K& key) {
    size_t hash = std::hash<K>{}(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && !(data[h].hash == hash && data[h].key == key)) h = (h + 1) & mask;
    if (data[h].state == state::Empty || data[h].state == state::Tombstone) return;
    else data[h].state = state::Tombstone;
    s -= 1;
}

template<typename K, typename T>
const T& hash_map<K, T>::at(const K& key) {
    size_t hash = std::hash<K>{}(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && !(data[h].hash == hash && data[h].key == key)) h = (h + 1) & mask;
    if (data[h].state == state::Empty || data[h].state == state::Tombstone) throw std::out_of_range("key not found");
    return data[h].data;
}

template<typename K, typename T>
bool hash_map<K, T>::contains(const K& key) {
    size_t hash = std::hash<K>{}(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && !(data[h].hash == hash && data[h].key == key)) h = (h + 1) & mask;
    return !(data[h].state == state::Empty || data[h].state == state::Tombstone);
}


template<typename K, typename T>
T& hash_map<K, T>::operator[](const K& key) {
    size_t hash = std::hash<K>{}(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && !(data[h].hash == hash && data[h].key == key)) h = (h + 1) & mask;
    if (data[h].state != state::Filled) {
        if (++s > capacity * 3 / 5) {
            resize();
            h = hash & mask;
            while (data[h].state == state::Filled) h = (h + 1) & mask;
        }
        data[h] = {key, {}, hash}; // if tombstone or empty, make new
    }
    return data[h].data;
}

template<typename K, typename T>
void hash_map<K, T>::resize() {
    size_t n = capacity;
    capacity *= 2;
    mask = capacity - 1;
    data_type* temp = new data_type[capacity]{};
    for (size_t idx = 0; idx < n; idx++) {
        if (data[idx].state == state::Filled) {
            size_t h = data[idx].hash & mask;
            while (temp[h].state == state::Filled) h = (h + 1) & mask;
            temp[h] = std::move(data[idx]);
        }
    }

    delete[] data;
    data = temp;
}
