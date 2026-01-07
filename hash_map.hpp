#pragma once
#include <memory>
#include <stdexcept>
#include <vector>

template <typename K, typename T>
struct val{
    K key;
    T data;
    val(K k, T d) : key{k}, data{d} {};
};

template <typename K, typename T>
class hash_map : public std::allocator<std::vector<val<K, T>>>{
    using data_type = std::vector<val<K, T>>;

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
    T    at(const K& key);             // getter
    T&   operator[](const K& key);              // subscript operator
    size_t size() const { return s; }
    size_t cap() const { return capacity; }
};

template<typename K, typename T>
hash_map<K, T>::hash_map() {
    data = hash_map::allocate(capacity);
    for (int i = 0; i < capacity; i++) {
        new (&data[i]) data_type();
    }
}

template<typename K, typename T>
hash_map<K, T>::~hash_map() {
    for (int i = 0; i < capacity; i++) {
        data[i].~data_type();
    }
    hash_map::deallocate(data, capacity);
}

template<typename K, typename T>
void hash_map<K, T>::insert(const K& key, const T& val) {
    size_t h = std::hash<K>{}(key) & mask;
    for (auto& v : data[h]) {
        if (v.key == key) {
            v.data = val;
            return;
        }
    }
    if (++s > capacity / 4 * 3) resize();
    data[h].emplace_back(key, val);
}

template<typename K, typename T>
void hash_map<K, T>::erase(const K& key) {
    size_t h = std::hash<K>{}(key) & mask;
    for (int i = 0; i < data[h].size(); i++) {
        if (data[h][i].key == key) {
            data[h].erase(data[h].begin() + i);
            s--;
            return;
        }
    }
}

template<typename K, typename T>
T hash_map<K, T>::at(const K& key) {
    size_t h = std::hash<K>{}(key) & mask;
    for (const auto& v : data[h]) {
        if (v.key == key) return v.data;
    }
    T d{};
    return d;
}
template<typename K, typename T>
T& hash_map<K, T>::operator[](const K& key) {
    size_t h = std::hash<K>{}(key) & mask;
    for (auto& v : data[h]) {
        if (v.key == key) return v.data;
    }
    T d{};
    if (++s > capacity / 4 * 3) resize();
    data[h].emplace_back(key, d);
    return data[h].back().data;
}

template<typename K, typename T>
void hash_map<K, T>::resize() {
    int n = capacity;
    capacity *= 2;
    mask = capacity - 1;
    data_type* temp = hash_map::allocate(capacity);
    for (int i = 0; i < capacity; i++) {
        new (&temp[i]) data_type();
    }
    for (int i = 0; i < n; i++) {
        for (const auto& val : data[i]) {
            size_t h = std::hash<K>{}(val.key) & mask;
            temp[h].emplace_back(val.key, val.data);
        }
        data[i].~data_type();
    }
    hash_map::deallocate(data, n);
    data = temp;
}
