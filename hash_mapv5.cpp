#include <cstdint>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include "rapidhash.h"
#include "hash_mapv5.hpp"

template<typename K, typename V>
hash_map<K, V>::hash_map() {
    slots = new slot_t[capacity_]{};
    ctrls = new ctrl_t[groups_]{};
    std::memset(ctrls, Empty, groups_ * GROUP_SIZE);
}

template<typename K, typename V>
hash_map<K, V>::~hash_map() {
    delete[] slots;
    delete[] ctrls;
}

template<typename K, typename V>
size_t hash_map<K, V>::hash_key(const K& key) {
    // Use concepts to determine how to hash key, will be decided at compile time
    if constexpr (Arithmetic<K>) {
        size_t x = static_cast<size_t>(key);
        x ^= x >> 32;
        x *= 0xd6e8feb86659fd93ULL;
        x ^= x >> 32;
        return x;
    }
    else if constexpr (Container<K>) {
        return rapidhash(key.data(), key.size() * sizeof(*key.data()));
    }
    else if constexpr (Copyable<K>) {
        return rapidhash(&key, sizeof(K));
    }
}

template<typename K, typename V>
void hash_map<K, V>::insert(const K& key, const V& val) {
    size_t hash = hash_key(key);
    size_t group_idx = H1(hash) & mask_;
    uint8_t ctrl_byte = H2(hash);

    while (true) {
        auto& group = ctrls[group_idx];
        for (int i = 0; i < GROUP_SIZE; i++) {
            if (group[i] == ctrl_byte) {
                size_t slot = group_idx * GROUP_SIZE + i;
                if constexpr (Arithmetic<K>) {
                    if (slots[slot].data.first == key) {
                        slots[slot].data.second = val;
                        return;
                    }
                } else {
                    if (slots[slot].hash == hash && slots[slot].data.first == key) {
                        slots[slot].data.second = val;
                        return;
                    }
                }
            }
            else if (group[i] == Empty) {
                group[i] = ctrl_byte;
                if constexpr (Arithmetic<K>) {
                    new (&slots[group_idx * GROUP_SIZE + i]) slot_t{key, val};
                } else {
                    new (&slots[group_idx * GROUP_SIZE + i]) slot_t{key, val, hash};
                }
                if (++size_ > capacity_ * 7 / 8) resize();
                ++used_;
                return;
            }
        }
        group_idx = (group_idx + 1) & mask_;
    }
}

template<typename K, typename V>
V& hash_map<K, V>::operator[](const K& key) {
    size_t hash = hash_key(key);
    size_t group_idx = H1(hash) & mask_;
    uint8_t ctrl_byte = H2(hash);
    size_t ctrl_offset = GROUP_SIZE;

    while (true) {
        auto& group = ctrls[group_idx];
        for (int i = 0; i < GROUP_SIZE; i++) {
            if (group[i] == ctrl_byte) {
                size_t slot = group_idx * GROUP_SIZE + i;
                if constexpr (Arithmetic<K>) {
                    if (slots[slot].data.first == key) return slots[slot].data.second;
                } else {
                    if (slots[slot].hash == hash && slots[slot].data.first == key) return slots[slot].data.second;
                }
            } else if (group[i] == Empty) {
                ctrl_offset = i;
                break;
            }
        }
        if (ctrl_offset < GROUP_SIZE) {
            group[ctrl_offset] = ctrl_byte;
            if constexpr (Arithmetic<K>) {
                new (&slots[group_idx * GROUP_SIZE + ctrl_offset]) slot_t{key, {}};
            } else {
                new (&slots[group_idx * GROUP_SIZE + ctrl_offset]) slot_t{key, {}, hash};
            }
            ++used_;
            if (++size_ > capacity_ * 7 / 8) {
                resize();
                group_idx = H1(hash) & mask_;
                while (true) {
                    auto group = ctrls[group_idx];
                    for (int i = 0; i < GROUP_SIZE; i++) {
                        if (group[i] == ctrl_byte) {
                            size_t slot = group_idx * GROUP_SIZE + i;
                            if constexpr (Arithmetic<K>) {
                                if (slots[slot].data.first == key) return slots[slot].data.second;
                            } else {
                                if (slots[slot].hash == hash && slots[slot].data.first == key) return slots[slot].data.second;
                            }
                        }
                    }
                    group_idx = (group_idx + 1) & mask_;
                }
            }
            return slots[group_idx * GROUP_SIZE + ctrl_offset].data.second;
        }
        group_idx = (group_idx + 1) & mask_;
    }
}

template<typename K, typename V>
std::pair<const K, V>& hash_map<K, V>::at(const K& key) {
    size_t hash = hash_key(key);
    size_t group_idx = H1(hash) & mask_;
    uint8_t ctrl_byte = H2(hash);

    while (true) {
        auto& group = ctrls[group_idx];
        for (int i = 0; i < GROUP_SIZE; i++) {
            if (group[i] == ctrl_byte) {
                size_t slot = group_idx * GROUP_SIZE + i;
                if constexpr (Arithmetic<K>) {
                    if (slots[slot].data.first == key) return slots[slot].data;
                } else {
                    if (slots[slot].hash == hash && slots[slot].data.first == key) return slots[slot].data;
                }
            } else if (group[i] == Empty) throw std::out_of_range("Key not found");
        }
        group_idx = (group_idx + 1) & mask_;
    }
}

template<typename K, typename V>
void hash_map<K, V>::erase(const K& key) {
    size_t hash = hash_key(key);
    size_t group_idx = H1(hash) & mask_;
    uint8_t ctrl_byte = H2(hash);

    while (true) {
        auto& group = ctrls[group_idx];
        for (int i = 0; i < GROUP_SIZE; i++) {
            if (group[i] == ctrl_byte) {
                size_t slot = group_idx * GROUP_SIZE + i;
                if constexpr (Arithmetic<K>) {
                    if (slots[slot].data.first == key) {
                        group[i] = Tombstone;
                        --size_;
                        return;
                    }
                } else {
                    if (slots[slot].hash == hash && slots[slot].data.first == key) {
                        group[i] = Tombstone;
                        --size_;
                        return;
                    }
                }
            } else if (group[i] == Empty) return;
        }
        group_idx = (group_idx + 1) & mask_;
    }
}

template<typename K, typename V>
void hash_map<K, V>::resize() {
    size_t old_c = capacity_, old_g = groups_;
    slot_t* old_slots = slots;
    ctrl_t* old_ctrls = ctrls;

    capacity_ *= 2;
    groups_ *= 2;
    mask_ = groups_ - 1;
    slots = new slot_t[capacity_]{};
    ctrls = new ctrl_t[groups_]{};
    std::memset(ctrls, Empty, groups_ * GROUP_SIZE);

    size_ = 0; used_ = 0;

    for (size_t gi = 0; gi < old_g; gi++) {
        for (int i = 0; i < GROUP_SIZE; i++) {
            if (old_ctrls[gi][i] != Empty && old_ctrls[gi][i] != Tombstone) {
                auto slot = old_slots[gi * GROUP_SIZE + i];
                // if (slot.data.first == 112) std::cout << "Here" << std::endl;
                size_t hash;
                if constexpr (Arithmetic<K>) {
                    hash = hash_key(slot.data.first);
                } else {
                    hash = slot.hash;
                }
                size_t group_idx = H1(hash) & mask_;
                uint8_t ctrl_byte = H2(hash);

                bool inserted = false;
                while (!inserted) {
                    auto& group = ctrls[group_idx];
                    for (int j = 0; j < GROUP_SIZE; j++) {
                        if (group[j] == Empty) {
                            group[j] = ctrl_byte;
                            new (&slots[group_idx * GROUP_SIZE + j]) slot_t(slot);
                            ++size_;
                            ++used_;


                            inserted = true;
                            break;
                        }
                    }
                    if (!inserted) {
                        group_idx = (group_idx + 1) & mask_;
                    }
                }
            }
        }
    }

    delete[] old_slots;
    delete[] old_ctrls;
}

template<typename K, typename V>
void hash_map<K, V>::print() {
    for (int i = 0; i < capacity_; i++) std::cout << slots[i].data.first << ": " << slots[i].data.second << std::endl;
}

template<typename K, typename V>
bool hash_map<K, V>::contains(const K& key) {
    size_t hash = hash_key(key);
    size_t group_idx = H1(hash) & mask_;
    uint8_t ctrl_byte = H2(hash);

    while (true) {
        auto& group = ctrls[group_idx];
        for (int i = 0; i < GROUP_SIZE; i++) {
            if (group[i] == ctrl_byte) {
                size_t slot = group_idx * GROUP_SIZE + i;
                if constexpr (Arithmetic<K>) {
                    if (slots[slot].data.first == key) return true;
                } else {
                    if (slots[slot].hash == hash && slots[slot].data.first == key) return true;
                }
            } else if (group[i] == Empty) return false;
        }
        group_idx = (group_idx + 1) & mask_;
    }
}
