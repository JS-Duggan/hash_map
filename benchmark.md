# Benchmark

Benchmarking my hash map against `std::unordered_map` and `ankerl::unordered_dense` (another Swiss table implementation). Results are from a pre-computed randomised input array with numeric and string keys. All hash maps are tested with the same input array on each run; values shown are averages across 10 runs. Input size is 10 M.

## Numeric Keys

### Insert

| Implementation             | Time         | Throughput     |
|----------------------------|--------------|----------------|
| `std::unordered_map`       | 1.658374 s   | 6.03 Mop/s     |
| `hash_map`                 | 381.87 ms    | 26.19 Mop/s    |
| `ankerl::unordered_dense`  | 633.32 ms    | 15.79 Mop/s    |

### Contains

| Implementation             | Time         | Throughput     |
|----------------------------|--------------|----------------|
| `std::unordered_map`       | 319.51 ms    | 31.30 Mop/s    |
| `hash_map`                 | 155.80 ms    | 64.19 Mop/s    |
| `ankerl::unordered_dense`  | 123.54 ms    | 80.95 Mop/s    |

### At

| Implementation             | Time         | Throughput     |
|----------------------------|--------------|----------------|
| `std::unordered_map`       | 349.10 ms    | 28.65 Mop/s    |
| `hash_map`                 | 132.11 ms    | 75.70 Mop/s    |
| `ankerl::unordered_dense`  | 139.31 ms    | 71.78 Mop/s    |

### Erase

| Implementation             | Time         | Throughput     |
|----------------------------|--------------|----------------|
| `std::unordered_map`       | 1.697774 s   | 5.89 Mop/s     |
| `hash_map`                 | 170.91 ms    | 58.51 Mop/s    |
| `ankerl::unordered_dense`  | 291.95 ms    | 34.25 Mop/s    |

## String Keys

### Insert

| Implementation             | Time         | Throughput     |
|----------------------------|--------------|----------------|
| `std::unordered_map`       | 2.161911 s   | 4.63 Mop/s     |
| `hash_map`                 | 688.65 ms    | 14.52 Mop/s    |
| `ankerl::unordered_dense`  | 991.24 ms    | 10.09 Mop/s    |

### Contains

| Implementation             | Time         | Throughput     |
|----------------------------|--------------|----------------|
| `std::unordered_map`       | 760.26 ms    | 13.15 Mop/s    |
| `hash_map`                 | 502.13 ms    | 19.91 Mop/s    |
| `ankerl::unordered_dense`  | 472.51 ms    | 21.16 Mop/s    |

### At

| Implementation             | Time         | Throughput     |
|----------------------------|--------------|----------------|
| `std::unordered_map`       | 769.49 ms    | 13.00 Mop/s    |
| `hash_map`                 | 555.11 ms    | 18.01 Mop/s    |
| `ankerl::unordered_dense`  | 466.72 ms    | 21.43 Mop/s    |

### Erase

| Implementation             | Time         | Throughput     |
|----------------------------|--------------|----------------|
| `std::unordered_map`       | 1.722704 s   | 5.80 Mop/s     |
| `hash_map`                 | 472.63 ms    | 21.16 Mop/s    |
| `ankerl::unordered_dense`  | 640.94 ms    | 15.60 Mop/s    |
