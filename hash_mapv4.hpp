#pragma once
#include <stdexcept>
#include <type_traits>
#include <utility>
#include "rapidhash.h"

enum class state : char { // char because 3 states means I only need one byte, and too big for 1 bit, they should hurry up and produce q-bits
    Filled = 'F',
    Empty = 'E',
    Tombstone = 'T'
};

 template<typename K>
 concept Container = requires(K t) {
     t.data();
     t.size();
 };

 template<typename K>
 concept Arithmetic = std::is_arithmetic_v<K>;

 template<typename K>
 concept Copyable = std::is_trivially_copyable_v<K> && !Arithmetic<K>;

 template<typename K, typename T>
 struct slot;

 // arithmetic types don't store hash
 template<typename K, typename T> requires Arithmetic<K>
 struct slot<K, T>
 {
     K key;
     T data;
     state state;

     slot(K k, T d) : key{k}, data{d}, state{state::Filled} {}
     slot() : key{}, data{}, state{state::Empty} {}
 };

// non arithmetic types do store hash
 template<typename K, typename T> requires (!Arithmetic<K>)
 struct slot<K, T>
 {
     K key;
     T data;
     size_t hash;
     state state;

     slot(K k, T d, size_t h) : key{std::move(k)}, data{std::move(d)}, hash{h}, state{state::Filled} {}
     slot() : key{}, data{}, hash{}, state{state::Empty} {}
 };


template <typename K, typename T>
class hash_map {
    using data_type = slot<K, T>;

    data_type* data;
    size_t capacity{128};                // default size, resizes at 75% full for amortized benefits etc, power of two to make use of bitmask
    size_t mask{capacity - 1};
    size_t s{0};
    size_t hash_key(const K& key) requires Arithmetic<K>; // define hash func for specialization
    size_t hash_key(const K& key) requires Container<K>;
    size_t hash_key(const K& key) requires Copyable<K>;
    void resize() requires Arithmetic<K>;
    void resize() requires (!Arithmetic<K>);
public:
    hash_map();                          // constructor
    ~hash_map();                         // destructor

    // Arithmetic versions of function
    void insert(const K& key, const T& val) requires Arithmetic<K>;  // setter
    void erase(const K& key) requires Arithmetic<K>;
    const T& at(const K& key) requires Arithmetic<K>;             // getter
    bool contains(const K& key) requires Arithmetic<K>;
    T& operator[](const K& key) requires Arithmetic<K>;              // subscript operator

    // Non-Arithmetic versions of functions
    void insert(const K& key, const T& val) requires (!Arithmetic<K>);  // setter
    void erase(const K& key) requires (!Arithmetic<K>);
    const T& at(const K& key) requires (!Arithmetic<K>);             // getter
    bool contains(const K& key) requires (!Arithmetic<K>);
    T& operator[](const K& key) requires (!Arithmetic<K>);              // subscript operator

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
size_t hash_map<K, T>::hash_key(const K& key) requires Arithmetic<K> {
    size_t x = static_cast<size_t>(key);
    x ^= x >> 32;
    x *= 0xd6e8feb86659fd93ULL;
    x ^= x >> 32;
    return x;
}

template<typename K, typename T>
size_t hash_map<K, T>::hash_key(const K& key) requires Container<K> {
    return rapidhash(key.data(), key.size() * sizeof(*key.data()));
}

template<typename K, typename T>
size_t hash_map<K, T>::hash_key(const K& key) requires Copyable<K> {
    rapidhash(&key, sizeof(T));
}

// ARITHMETIC

template<typename K, typename T>
void hash_map<K, T>::insert(const K& key, const T& val) requires Arithmetic<K> {
    size_t h = hash_key(key) & mask;
    while (data[h].state != state::Empty && data[h].key != key) h = (h + 1) & mask;
    bool inserted = (data[h].state != state::Filled);
    data[h] = {key, val};
    if (inserted && ++s > capacity * 3 / 5) resize(); // I have gone with smaller load percentage for open addressing
}

template<typename K, typename T>
void hash_map<K, T>::erase(const K& key) requires Arithmetic<K> {
    size_t h = hash_key(key) & mask;
    while (data[h].state != state::Empty && data[h].key != key) h = (h + 1) & mask;
    if (data[h].state == state::Empty || data[h].state == state::Tombstone) return;
    else data[h].state = state::Tombstone;
    s -= 1;
}

template<typename K, typename T>
const T& hash_map<K, T>::at(const K& key) requires Arithmetic<K> {
    size_t h = hash_key(key) & mask;
    while (data[h].state != state::Empty && data[h].key != key) h = (h + 1) & mask;
    if (data[h].state == state::Empty || data[h].state == state::Tombstone) throw std::out_of_range("key not found");
    return data[h].data;
}

template<typename K, typename T>
bool hash_map<K, T>::contains(const K& key) requires Arithmetic<K> {
    size_t hash = hash_key(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && data[h].key == key) h = (h + 1) & mask;
    return !(data[h].state == state::Empty || data[h].state == state::Tombstone);
}


template<typename K, typename T>
T& hash_map<K, T>::operator[](const K& key) requires Arithmetic<K> {
    size_t hash = hash_key(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && data[h].key != key) h = (h + 1) & mask;
    if (data[h].state != state::Filled) {
        if (++s > capacity * 3 / 5) {
            resize();
            h = hash & mask;
            while (data[h].state == state::Filled) h = (h + 1) & mask;
        }
        data[h] = {key, {}}; // if tombstone or empty, make new
    }
    return data[h].data;
}

template<typename K, typename T>
void hash_map<K, T>::resize() requires Arithmetic<K> {
    size_t n = capacity;
    capacity *= 2;
    mask = capacity - 1;
    data_type* temp = new data_type[capacity]{};
    for (size_t idx = 0; idx < n; idx++) {
        if (data[idx].state == state::Filled) {
            size_t h = hash_key(data[idx].key) & mask;
            while (temp[h].state == state::Filled) {
                h = (h + 1) & mask;
            }
            temp[h] = std::move(data[idx]);
        }
    }
    delete[] data;
    data = temp;
}

// NON ARITHMETIC
template<typename K, typename T>
void hash_map<K, T>::insert(const K& key, const T& val) requires (!Arithmetic<K>) {
    size_t hash = hash_key(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && !(data[h].hash == hash && data[h].key == key)) h = (h + 1) & mask;
    bool inserted = (data[h].state != state::Filled);
    data[h] = {key, val, hash};
    if (inserted && ++s > capacity * 3 / 5) resize(); // I have gone with smaller load percentage for open addressing
}

template<typename K, typename T>
void hash_map<K, T>::erase(const K& key) requires (!Arithmetic<K>) {
    size_t hash = hash_key(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && !(data[h].hash == hash && data[h].key == key)) h = (h + 1) & mask;
    if (data[h].state == state::Empty || data[h].state == state::Tombstone) return;
    else data[h].state = state::Tombstone;
    s -= 1;
}

template<typename K, typename T>
const T& hash_map<K, T>::at(const K& key) requires (!Arithmetic<K>) {
    size_t hash = hash_key(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && !(data[h].hash == hash && data[h].key == key)) h = (h + 1) & mask;
    if (data[h].state == state::Empty || data[h].state == state::Tombstone) throw std::out_of_range("key not found");
    return data[h].data;
}

template<typename K, typename T>
bool hash_map<K, T>::contains(const K& key) requires (!Arithmetic<K>) {
    size_t hash = hash_key(key);
    size_t h = hash & mask;
    while (data[h].state != state::Empty && !(data[h].hash == hash && data[h].key == key)) h = (h + 1) & mask;
    return !(data[h].state == state::Empty || data[h].state == state::Tombstone);
}


template<typename K, typename T>
T& hash_map<K, T>::operator[](const K& key) requires (!Arithmetic<K>) {
    size_t hash = hash_key(key);
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
void hash_map<K, T>::resize() requires (!Arithmetic<K>){
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
