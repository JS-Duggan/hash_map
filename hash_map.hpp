#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

enum ctrl : uint8_t
{                     // use 1 byte int, 0 comparison faster
    Empty = 0xFF,     // 0b11111111
    Tombstone = 0xFE, // 0b11111110
    Sentinel = 0x80   // 0b10000000
                      // Filled = 0b0xxxxxxx
};

template <typename K>
concept Container = requires(K key) {
    key.data();
    key.size();
};

template <typename K>
concept Arithmetic = std::is_arithmetic_v<K>;

template <typename K>
concept Copyable = std::is_trivially_copyable_v<K> && !Arithmetic<K>;

template <typename K, typename V> struct slot;

// arithmetic types don't store hash
template <typename K, typename V>
    requires Arithmetic<K>
struct slot<const K, V>
{
    std::pair<const K, V> data;
    slot(const K &key, const V &val) : data{key, val} {}
    slot() : data{} {}
};

// non arithmetic types do store hash
template <typename K, typename V>
    requires(!Arithmetic<K>)
struct slot<K, V>
{
    std::pair<K, V> data;
    size_t hash;
    slot(const K &key, const V &val, size_t hash) : data{key, val}, hash{hash} {}
    slot() : data{}, hash{} {}
};

template <typename K, typename V> class hash_map
{
    // constant values
    static constexpr size_t k_group_size_{16};        // size of SIMD register
    static constexpr uint16_t k_group_mask_{0xFFFF};  // 16 bit all set to 1
    static constexpr size_t k_default_capacity_{128}; // default starting capacity

    using slot_t = slot<const K, V>;
    using ctrl_t = uint8_t[k_group_size_];

    slot_t *slots;
    ctrl_t *ctrls;

    // capacity is linked to groups, because SIMD instructions look at 16
    // bits at a time, groups are size 16, capacity is 16 * groups
    size_t capacity_;
    size_t groups_;
    size_t mask_;
    size_t size_{0};
    size_t used_{0};
    inline size_t hash_key(const K &key) const;
    void resize();
    bool at_max_load() const { return used_ > capacity_ - (capacity_ >> 3); }

    // utility functions to get H1 and H2
    size_t H1(size_t hash) const { return hash >> 7; }
    size_t H2(size_t hash) const { return hash & 0x7F; }

  public:
    // constructors
    hash_map(size_t num_groups = k_default_capacity_);
    ~hash_map(); // destructor

    V &operator[](const K &key);
    void insert(const K &key, const V &val);
    void erase(const K &key);
    std::pair<const K, V> &at(const K &key) const;
    bool contains(const K &key) const;

    size_t size() const { return size_; }
    size_t used() const { return used_; }
    size_t capacity() const { return capacity_; }
};
