#pragma once
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

template <typename K, typename T>
struct slot{
    K key;
    T data;
    size_t hash;
    slot(K k, T d, size_t h) : key{k}, data{d}, hash{h} {};
    slot() : key{}, data{}, hash{} {};
};

template <typename K, typename T>
class hash_map {
    using data_type = std::vector<slot<K, T>>;

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
    data = new data_type[capacity];
}

template<typename K, typename T>
hash_map<K, T>::~hash_map() {
    delete[] data;
}

template<typename K, typename T>
void hash_map<K, T>::insert(const K& key, const T& val) {
    size_t hash = std::hash<K>{}(key);
    size_t h = hash & mask;
    for (auto& v : data[h]) {
        if (v.hash == hash && v.key == key) {
            v.data = val;
            return;
        }
    }
    data[h].emplace_back(key, val, hash);
    if (++s > capacity / 4 * 3) resize();
}

template<typename K, typename T>
void hash_map<K, T>::erase(const K& key) {
    size_t hash = std::hash<K>{}(key);
    size_t h = hash & mask;
    for (size_t i = 0; i < data[h].size(); i++) {
        if (data[h][i].hash == hash && data[h][i].key == key) {
            data[h].erase(data[h].begin() + i);
            s--;
            return;
        }
    }
}

template<typename K, typename T>
const T& hash_map<K, T>::at(const K& key) {
    size_t hash = std::hash<K>{}(key);
    size_t h = hash & mask;
    for (const auto& v : data[h]) {
        if (v.hash == hash && v.key == key) return v.data;
    }
    throw std::out_of_range("key not found");
}

template<typename K, typename T>
bool hash_map<K, T>::contains(const K& key) {
    size_t hash = std::hash<K>{}(key);
    size_t h = hash & mask;
    for (const auto& v : data[h]) {
        if (v.hash == hash && v.key == key) return true;
    }
    return false;
}


template<typename K, typename T>
T& hash_map<K, T>::operator[](const K& key) {
    size_t hash = std::hash<K>{}(key);
    size_t h = hash & mask;
    for (auto& v : data[h]) {
        if (v.hash == hash && v.key == key) return v.data;
    }
    T d{};
    data[h].emplace_back(key, d, hash);
    if (++s > capacity / 4 * 3) resize();
    return data[hash & mask].back().data;
}

template<typename K, typename T>
void hash_map<K, T>::resize() {
    size_t n = capacity;
    capacity *= 2;
    mask = capacity - 1;
    data_type* temp = new data_type[capacity];
    for (size_t i = 0; i < n; i++) {
        for (const auto& val : data[i]) {
            size_t h = val.hash & mask;
            temp[h].push_back(val);
        }
    }
    delete[] data;
    data = temp;
}
