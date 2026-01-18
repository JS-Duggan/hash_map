# Hash Map

I have made a serious attempt to create a high-performance hash map implementation. I used the Swiss table algorithm with SIMD-accelerated probing. Inspired by Google's Abseil `flat_hash_map`.

## Features

- **SIMD-accelerated lookups**: Uses SSE2 (x86) or NEON (ARM) to probe 16 slots in parallel
- **Compile-time key dispatch**: Uses C++20 concepts to optimize hashing for arithmetic, container, and trivially copyable types, with compile time decisions to avoid branching
- **Cache-friendly**: Flat memory layout with control bytes separated from data slots
- **Low memory overhead**: 1-byte control metadata per slot

## Usage

```cpp
#include "hash_map.cpp"

// Integer keys
hash_map<int, std::string> scores;
scores.insert(1, "alice");
scores[2] = "bob";

if (scores.contains(1)) {
    auto& entry = scores.at(1);
}

scores.erase(1);

// String keys
hash_map<std::string, int> ages;
ages.insert("alice", 30);
ages["bob"] = 25;
```

## API

| Method | Description |
|--------|-------------|
| `insert(key, val)` | Insert or update a key-value pair |
| `at(key)` | Access element (throws `std::out_of_range` if missing) |
| `operator[key]` | Access element (inserts default value if missing) |
| `contains(key)` | Returns `true` if key exists |
| `erase(key)` | Remove element by key |
| `size()` | Number of stored elements |
| `capacity()` | Total slot capacity |


### Dependencies

- `rapidhash.h` - Fast hashing for non-arithmetic types
- `sse2neon.h` - NEON translation layer for ARM (Apple Silicon)

For x86, uncomment `#include <emmintrin.h>` in `hash_map.cpp`.

## Benchmarks

Tested with 10M randomized operations, averaged over 10 runs.

### Numeric Keys

| Operation | `std::unordered_map` | `hash_map` | `ankerl::unordered_dense` |
|-----------|----------------------|------------|---------------------------|
| Insert    | 6.03 Mop/s           | **26.19 Mop/s** | 15.79 Mop/s          |
| Contains  | 31.30 Mop/s          | 64.19 Mop/s | **80.95 Mop/s**          |
| At        | 28.65 Mop/s          | **75.70 Mop/s** | 71.78 Mop/s          |
| Erase     | 5.89 Mop/s           | **58.51 Mop/s** | 34.25 Mop/s          |

### String Keys

| Operation | `std::unordered_map` | `hash_map` | `ankerl::unordered_dense` |
|-----------|----------------------|------------|---------------------------|
| Insert    | 4.63 Mop/s           | **14.52 Mop/s** | 10.09 Mop/s          |
| Contains  | 13.15 Mop/s          | 19.91 Mop/s | **21.16 Mop/s**          |
| At        | 13.00 Mop/s          | 18.01 Mop/s | **21.43 Mop/s**          |
| Erase     | 5.80 Mop/s           | **21.16 Mop/s** | 15.60 Mop/s          |

## Implementation Details

### Swiss Table Algorithm

The hash map uses control bytes to enable SIMD parallel matching:

- Each slot has a 1-byte control: `Empty (0xFF)`, `Tombstone (0xFE)`, or `H2 (0x00-0x7F)`
  - The first bit signals if it is empty 1, or filled 0
  - H2 is the lower 7 bits of the hash, used as a hint with the remaining 7 bits of the control byte
- H1 (remaining bits) determines the starting group for probing
- SIMD compares 16 control bytes simultaneously to find matches

### Key Type Handling

Uses C++20 concepts for compile-time dispatch:

| Concept | Types | Hashing |
|---------|-------|---------|
| `Arithmetic` | `int`, `float`, `double`, etc. | SplitMix64 finalizer |
| `Container` | `std::string`, `std::vector`, etc. | rapidhash on `.data()` |
| `Copyable` | Other trivially copyable types | rapidhash on bytes |

Arithmetic keys don't store the hash (recomputed cheaply), saving 8 bytes per slot.

---

## Development History

### v1
Naive chaining hash map. Worked well enough but did not beat `std::unordered_map`.

### v2
Still chaining, attempted to store hashes to avoid recomputation. Slower on benchmarks, likely because the string benchmarks used small strings and integers.

### v3
Flat map implementation using C-style arrays for full control over resizing. Outperformed `std::unordered_map` on datasets n <= 10M, but struggled at >= 100M due to hash storage overhead.

### v4
Introduced template specializations for slots based on type - only storing the hash for non-numeric types (since numeric types hash in O(1)). Switched to separate hashing algorithms: SplitMix64 for numeric types, rapidhash for others. The numeric hash is slower but avoids clustering, performing better on larger random workloads.

### v5.1
Added control bytes for efficient searching/probing in preparation for SIMD. Performed worse than regular linear probing due to per-byte checking overhead.

### v5.2
Implemented SIMD instructions for simultaneous group searching. Now significantly faster, combining all learned concepts into a Swiss table implementation.
