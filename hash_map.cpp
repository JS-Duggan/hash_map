#include "hash_map.hpp"
#include "rapidhash.h"
#include "sse2neon.h" // this is for apple sillicon, otherwise use emmintrin
// #include <emmintrin.h>
#include <cstdint>
#include <cstring>
#include <new> // IWYU pragma: keep (placement new for c++23 >= compilation)
#include <stdexcept>

template <typename K, typename V>
hash_map<K, V>::hash_map(size_t num_groups)
    : groups_{num_groups}, capacity_{k_group_size_ * num_groups}, mask_{num_groups - 1}
{
    slots = new slot_t[capacity_]{};
    ctrls = new ctrl_t[groups_]{};
    std::memset(ctrls, Empty, groups_ * k_group_size_);
}

template <typename K, typename V> hash_map<K, V>::~hash_map()
{
    delete[] slots;
    delete[] ctrls;
}

template <typename K, typename V> inline size_t hash_map<K, V>::hash_key(const K &key) const
{
    // Use concepts to determine how to hash key, will be decided at compile time
    if constexpr (Arithmetic<K>)
    {
        // SplitMix64 finalizer - mixes bits to improve hash distribution
        // See: https://prng.di.unimi.it/splitmix64.c
        constexpr size_t kSplitMixMultiplier = 0xd6e8feb86659fd93ULL;
        constexpr int kMixShift = 32;

        size_t hash = static_cast<size_t>(key);
        hash ^= hash >> kMixShift;
        hash *= kSplitMixMultiplier;
        hash ^= hash >> kMixShift;
        return hash;
    }
    else if constexpr (Container<K>)
    {
        return rapidhash(key.data(), key.size() * sizeof(*key.data()));
    }
    else if constexpr (Copyable<K>)
    {
        return rapidhash(&key, sizeof(K));
    }
}

inline uint16_t match(uint8_t *group, uint8_t ctrl_byte)
{
    __m128i ctrl = _mm_loadu_si128((__m128i *)group); // load group into register
    __m128i match = _mm_set1_epi8(ctrl_byte);         // create mask of control byte
    __m128i cmp = _mm_cmpeq_epi8(ctrl, match);        // compare both 16 byte 'arrays' simultaneously
    return _mm_movemask_epi8(cmp);                    // compress to a 16bit integer 1 - match, 0 - miss
}

template <typename K, typename V> void hash_map<K, V>::insert(const K &key, const V &val)
{
    size_t hash = hash_key(key);
    size_t group_idx = H1(hash) & mask_;
    uint8_t ctrl_byte = H2(hash);

    while (true)
    {
        auto &group = ctrls[group_idx];
        uint16_t ctrl_mask = match(group, ctrl_byte);

        while (ctrl_mask != 0)
        {
            int offset = __builtin_ctz(ctrl_mask); // count trailing zeroes, index of first bit that matches
            size_t slot_idx = (group_idx * k_group_size_) + offset;

            if constexpr (Arithmetic<K>)
            {
                if (slots[slot_idx].data.first == key)
                {
                    slots[slot_idx].data.second = val;
                    return;
                }
            }
            else
            {
                if (slots[slot_idx].hash == hash && slots[slot_idx].data.first == key)
                {
                    slots[slot_idx].data.second = val;
                    return;
                }
            }

            ctrl_mask &= (ctrl_mask - 1); // clear lowest set bit
        }

        uint16_t empty_mask = match(group, Empty);
        if (empty_mask != 0)
        {
            int offset = __builtin_ctz(empty_mask);
            group[offset] = ctrl_byte;
            size_t slot_idx = (group_idx * k_group_size_) + offset;
            if constexpr (Arithmetic<K>)
            {
                new (&slots[slot_idx]) slot_t{key, val};
            }
            else
            {
                ::new (&slots[slot_idx]) slot_t{key, val, hash};
            }
            size_++;
            used_++;
            if (at_max_load())
            {
                resize();
            }
            return;
        }
        group_idx = (group_idx + 1) & mask_;
    }
}

template <typename K, typename V> V &hash_map<K, V>::operator[](const K &key)
{
    size_t hash = hash_key(key);
    // re-size early if an insert would go above load factor
    // this prevents re-calculating the slot, while only resizing
    // one value early, which I think is probably a fine trade-off
    if (used_ + 1 > capacity_ - (capacity_ >> 3))
    {
        resize();
    }
    size_t group_idx = H1(hash) & mask_;
    uint8_t ctrl_byte = H2(hash);
    size_t ctrl_offset = k_group_size_;

    while (true)
    {
        auto &group = ctrls[group_idx];
        uint16_t ctrl_mask = match(group, ctrl_byte);

        while (ctrl_mask != 0)
        {
            int offset = __builtin_ctz(ctrl_mask); // count trailing zeroes, index of first bit that matches
            size_t slot_idx = (group_idx * k_group_size_) + offset;

            if constexpr (Arithmetic<K>)
            {
                if (slots[slot_idx].data.first == key)
                {
                    return slots[slot_idx].data.second;
                }
            }
            else
            {
                if (slots[slot_idx].hash == hash && slots[slot_idx].data.first == key)
                {
                    return slots[slot_idx].data.second;
                }
            }

            ctrl_mask &= (ctrl_mask - 1); // clear lowest set bit
        }

        uint16_t empty_mask = match(group, Empty);
        if (empty_mask != 0)
        {
            int offset = __builtin_ctz(empty_mask);
            group[offset] = ctrl_byte;
            size_t slot_idx = (group_idx * k_group_size_) + offset;
            if constexpr (Arithmetic<K>)
            {
                ::new (&slots[slot_idx]) slot_t{key, {}};
            }
            else
            {
                ::new (&slots[slot_idx]) slot_t{key, {}, hash};
            }
            size_++;
            used_++;
            return slots[slot_idx].data.second;
        }
        group_idx = (group_idx + 1) & mask_;
    }
}

template <typename K, typename V> std::pair<const K, V> &hash_map<K, V>::at(const K &key) const
{
    size_t hash = hash_key(key);
    size_t group_idx = H1(hash) & mask_;
    uint8_t ctrl_byte = H2(hash);

    while (true)
    {
        auto &group = ctrls[group_idx];
        uint16_t ctrl_mask = match(group, ctrl_byte);

        while (ctrl_mask != 0)
        {
            int offset = __builtin_ctz(ctrl_mask); // count trailing zeroes, index of first bit that matches
            size_t slot_idx = (group_idx * k_group_size_) + offset;

            if constexpr (Arithmetic<K>)
            {
                if (slots[slot_idx].data.first == key)
                {
                    return slots[slot_idx].data;
                }
            }
            else
            {
                if (slots[slot_idx].hash == hash && slots[slot_idx].data.first == key)
                {
                    return slots[slot_idx].data;
                }
            }

            ctrl_mask &= (ctrl_mask - 1); // clear lowest set bit
        }

        uint16_t empty_mask = match(group, Empty);
        if (empty_mask != 0)
        {
            throw std::out_of_range("Key not found");
        }
        group_idx = (group_idx + 1) & mask_;
    }
}

template <typename K, typename V> bool hash_map<K, V>::contains(const K &key) const
{
    size_t hash = hash_key(key);
    size_t group_idx = H1(hash) & mask_;
    uint8_t ctrl_byte = H2(hash);

    while (true)
    {
        auto &group = ctrls[group_idx];
        uint16_t ctrl_mask = match(group, ctrl_byte);

        while (ctrl_mask != 0)
        {
            int offset = __builtin_ctz(ctrl_mask); // count trailing zeroes, index of first bit that matches
            size_t slot_idx = (group_idx * k_group_size_) + offset;

            if constexpr (Arithmetic<K>)
            {
                if (slots[slot_idx].data.first == key)
                {
                    return true;
                }
            }
            else
            {
                if (slots[slot_idx].hash == hash && slots[slot_idx].data.first == key)
                {
                    return true;
                }
            }

            ctrl_mask &= (ctrl_mask - 1); // clear lowest set bit
        }

        uint16_t empty_mask = match(group, Empty);
        if (empty_mask != 0)
        {
            return false;
        }
        group_idx = (group_idx + 1) & mask_;
    }
}

template <typename K, typename V> void hash_map<K, V>::erase(const K &key)
{
    size_t hash = hash_key(key);
    size_t group_idx = H1(hash) & mask_;
    uint8_t ctrl_byte = H2(hash);

    while (true)
    {
        auto &group = ctrls[group_idx];
        uint16_t ctrl_mask = match(group, ctrl_byte);

        while (ctrl_mask != 0)
        {
            int offset = __builtin_ctz(ctrl_mask); // count trailing zeroes, index of first bit that matches
            size_t slot_idx = (group_idx * k_group_size_) + offset;

            if constexpr (Arithmetic<K>)
            {
                if (slots[slot_idx].data.first == key)
                {
                    group[offset] = Tombstone;
                    size_--;
                    return;
                }
            }
            else
            {
                if (slots[slot_idx].hash == hash && slots[slot_idx].data.first == key)
                {
                    group[offset] = Tombstone;
                    size_--;
                    return;
                }
            }

            ctrl_mask &= (ctrl_mask - 1); // clear lowest set bit
        }

        uint16_t empty_mask = match(group, Empty);
        if (empty_mask != 0)
        {
            return;
        }
        group_idx = (group_idx + 1) & mask_;
    }
}

template <typename K, typename V> void hash_map<K, V>::resize()
{
    size_t old_g = groups_;
    slot_t *old_slots = slots;
    ctrl_t *old_ctrls = ctrls;

    capacity_ *= 2;
    groups_ *= 2;
    mask_ = groups_ - 1;
    slots = new slot_t[capacity_]{};
    ctrls = new ctrl_t[groups_]{};
    std::memset(ctrls, Empty, groups_ * k_group_size_);

    size_ = 0;
    used_ = 0;

    for (size_t gi = 0; gi < old_g; gi++)
    {
        // loadu_si128 load control bytes into SIMD register
        // movemask_epi8 extract MSB from 16 bytes into a 16 bit result
        // ~ bitwise flips so that Filled slots are 1, Empty and Tombstone are 0
        // &k_group_mask_ keep the 16 filled bits
        // filled: bitmask where 1 bit indicates filled slot to be re-hashed
        uint16_t filled = ~_mm_movemask_epi8(_mm_loadu_si128((__m128i *)old_ctrls[gi])) & k_group_mask_;
        while (filled != 0)
        {
            int old_offset = __builtin_ctz(filled);
            auto slot_data = old_slots[(gi * k_group_size_) + old_offset];
            size_t hash;
            if constexpr (Arithmetic<K>)
            {
                hash = hash_key(slot_data.data.first);
            }
            else
            {
                hash = slot_data.hash;
            }
            size_t group_idx = H1(hash) & mask_;
            uint8_t ctrl_byte = H2(hash);

            while (true)
            {
                auto &group = ctrls[group_idx];
                uint16_t empty_mask = match(group, Empty);
                if (empty_mask != 0)
                {
                    int offset = __builtin_ctz(empty_mask);
                    group[offset] = ctrl_byte;
                    size_t slot_idx = (group_idx * k_group_size_) + offset;
                    ::new (&slots[slot_idx]) slot_t{slot_data};
                    size_++;
                    used_++;
                    break;
                }
                group_idx = (group_idx + 1) & mask_;
            }
            filled &= (filled - 1);
        }
    }

    delete[] old_slots;
    delete[] old_ctrls;
}
