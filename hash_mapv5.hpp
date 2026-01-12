#include <type_traits>
#include <utility>
#include <cstdint>
#include <cstddef>

#define GROUP_SIZE 16

enum ctrl : uint8_t { // use 1 byte int, 0 comparison faster
    Empty = 0xFF,     // 0b11111111
    Tombstone = 0xFE, // 0b11111110
    Sentinel = 0x80   // 0b10000000
    // Filled =          0b0xxxxxxx
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

 template<typename K, typename V>
 struct slot;

 // arithmetic types don't store hash
 template<typename K, typename V> requires Arithmetic<K>
 struct slot<K, V>
 {
     std::pair<const K, V> data;
     slot(const K& k, const V& d) : data{k, d} {}
     slot() : data{} {}
 };

// non arithmetic types do store hash
 template<typename K, typename V> requires (!Arithmetic<K>)
 struct slot<K, V>
 {
     std::pair<const K, V> data;
     size_t hash;
     slot(const K& k, const V& d, size_t h) : data{k, d}, hash{h} {}
     slot() : data{}, hash{} {}
 };

template <typename K, typename V>
class hash_map {
    using slot_t = slot<const K, V>;
    using ctrl_t = uint8_t[GROUP_SIZE];

    slot_t* slots;
    ctrl_t* ctrls;

    // capacity is linked to groups, because SIMD instructions look at 16
    // bits at a time, groups are size 16, capacity is 16 * groups
    size_t capacity_{128};
    size_t groups_{8};
    size_t mask_{groups_ - 1};
    size_t size_{0};
    size_t used_{0};
    size_t hash_key(const K& key);
    void resize();

    // utility functions to get H1 and H2
    size_t H1(size_t hash) { return hash >> 7; }
    size_t H2(size_t hash) { return hash & 0x7F; }
public:
    hash_map();                          // constructors
    hash_map(size_t num_groups) : groups_{num_groups}, capacity_{GROUP_SIZE * num_groups}, mask_{num_groups - 1} {};
    ~hash_map();                         // destructor


    void insert(const K& key, const V& val);
    void erase(const K& key);
    std::pair<const K, V>& at(const K& key);
    V& operator[](const K& key);
    bool contains(const K& key);

    void print();

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
};
