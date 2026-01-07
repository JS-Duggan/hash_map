#pragma once
#include <memory>
#include <stdexcept>
#include <vector>

using namespace std;
template <typename K, typename T>
struct val{
    K key;
    T data;
    val(K k, T d) : key{k}, data{d} {};
};

template <typename K, typename T>
class hash_map : public allocator<vector<val<K, T>>>{
    using data_type = vector<val<K, T>>*;

    data_type data;
    size_t capacity{128};                // default size, resizes at 75% full for amortized benefits etc, power of two to make use of bitmask
    size_t mask{capacity - 1};
    size_t size{0};
    void resize();
public: 
    hash_map();                          // constructor
    ~hash_map();                         // destructor
    void insert(K key, T val);  // setter
    T    at(K key);             // getter
    T&   operator[](K key);              // subscript operator
};

template<typename K, typename T>
hash_map<K, T>::hash_map() {
    data = hash_map::allocate(capacity);
}

template<typename K, typename T>
hash_map<K, T>::~hash_map() {
    hash_map::deallocate(data, capacity);
}

template<typename K, typename T>
void hash_map<K, T>::insert(K key, T val) {
    size_t h = hash<K>{}(key) & mask;
    if (++size > capacity / 4 * 3) resize();
    data[h].emplace_back(key, val);
}

template<typename K, typename T>
T hash_map<K, T>::at(K key) {
    size_t h = hash<K>{}(key) & mask;
    for (const auto& v : data[h]) {
        if (v.key == key) return v.data;
    }
    return end;
}
template<typename K, typename T>
T& hash_map<K, T>::operator[](K key) {
    size_t h = hash<K>{}(key) & mask;
    for (auto& v : data[h]) {
        if (v.key == key) return v.data;
    }
    T d{};
    insert(key, d);
    return operator[](key);
}

template<typename K, typename T>
void hash_map<K, T>::resize() {
    int n = capacity;
    capacity *= 2;
    mask = capacity - 1;
    data_type temp = hash_map::allocate(capacity);
    for (int i = 0; i < n; i++) {
        for (const auto& val : data[i]) {
            size_t h = hash<K>{}(val.key) & mask;
            temp[h].emplace_back(val.key, val.data);
        }
    }
}







