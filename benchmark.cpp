#include <iostream>
#include <iomanip>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include "hash_mapv5.cpp"

constexpr size_t N = 100000000;

void print_result(const std::string& name, double ns, size_t ops) {
    double op_per_sec = ops / (ns / 1e9);

    std::string time_str;
    if (ns >= 1e9) {
        time_str = std::to_string(ns / 1e9) + " s";
    } else if (ns >= 1e6) {
        time_str = std::to_string(ns / 1e6) + " ms";
    } else if (ns >= 1e3) {
        time_str = std::to_string(ns / 1e3) + " us";
    } else {
        time_str = std::to_string(ns) + " ns";
    }

    std::string ops_str;
    if (op_per_sec >= 1e9) {
        ops_str = std::to_string(op_per_sec / 1e9) + " G";
    } else if (op_per_sec >= 1e6) {
        ops_str = std::to_string(op_per_sec / 1e6) + " M";
    } else if (op_per_sec >= 1e3) {
        ops_str = std::to_string(op_per_sec / 1e3) + " K";
    } else {
        ops_str = std::to_string(op_per_sec) + " ";
    }

    std::cout << std::left << std::setw(30) << name
              << std::setw(20) << time_str
              << ops_str << "op/s\n";
}

int main() {
    std::cout << std::left << std::setw(30) << "Operation"
              << std::setw(20) << "Time" << "Throughput\n";
    std::cout << std::string(70, '-') << "\n";

    std::mt19937_64 rng(42);
    std::uniform_int_distribution<size_t> dist(0, N * 2);

    std::vector<size_t> keys(N);
    for (size_t i = 0; i < N; i++) {
        keys[i] = dist(rng);
    }
    // ===== INSERT =====

    {
        std::unordered_map<size_t, size_t> um;
        auto start = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < N; i++) {
            um.insert({keys[i], i});
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map insert", duration.count(), N);
    }
    {
        hash_map<size_t, size_t> hm;
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            hm.insert(keys[i], i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map insert", duration.count(), N);
    }
    std::cout << "\n";

    // ===== CONTAINS =====
    auto lookup_keys = keys;
    std::shuffle(lookup_keys.begin(), lookup_keys.end(), rng);

    {
        std::unordered_map<size_t, size_t> um;
        for (size_t k : keys) um[k] = k;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t k : lookup_keys) {
            volatile bool found = um.contains(k);
            (void)found;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map contains", duration.count(), N);
    }
    {
        hash_map<size_t, size_t> hm;
        for (size_t k : keys) hm.insert(k, k);

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t k : lookup_keys) {
            volatile bool found = hm.contains(k);
            (void)found;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map contains", duration.count(), N);
    }
    std::cout << "\n";

    // ===== AT =====
    {
        std::unordered_map<size_t, size_t> um;
        for (size_t k : keys) um[k] = k;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t k : lookup_keys) {
            volatile size_t at = um.at(k);
            (void)at;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map at", duration.count(), N);
    }
    {
        hash_map<size_t, size_t> hm;
        for (size_t k : keys) hm.insert(k, k);

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t k : lookup_keys) {
            volatile auto at = hm.at(k);
            (void)at;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map at", duration.count(), N);
    }
    std::cout << "\n";

    // ===== ERASE =====
    auto erase_keys = keys;
    std::shuffle(erase_keys.begin(), erase_keys.end(), rng);

    {
        std::unordered_map<size_t, size_t> um;
        for (size_t k : keys) um[k] = k;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t k : erase_keys) {
            um.erase(k);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map erase", duration.count(), N);
    }
    {
        hash_map<size_t, size_t> hm;
        for (size_t k : keys) hm.insert(k, k);

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t k : erase_keys) {
            hm.erase(k);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map erase", duration.count(), N);
    }

    std::cout << "\n========== STRING KEY BENCHMARKS ==========\n\n";

    // Pre-generate string keys to avoid measuring string creation time
    std::vector<std::string> string_keys(N);
    for (size_t i = 0; i < N; i++) {
        size_t k = dist(rng);
        string_keys[i] = std::to_string(k);
    }

    // ===== STRING INSERT =====
    {
        std::unordered_map<std::string, size_t> um;
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            um.insert({string_keys[i], i});
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map<string> insert", duration.count(), N);
    }
    {
        hash_map<std::string, size_t> hm;
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            hm.insert(string_keys[i], i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map<string> insert", duration.count(), N);
    }
    std::cout << "\n";

    // ===== STRING CONTAINS =====
    {
        std::unordered_map<std::string, size_t> um;
        for (size_t i = 0; i < N; i++) um[string_keys[i]] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            volatile bool found = um.count(string_keys[i]) > 0;
            (void)found;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map<string> contains", duration.count(), N);
    }
    {
        hash_map<std::string, size_t> hm;
        for (size_t i = 0; i < N; i++) hm[string_keys[i]] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            volatile bool found = hm.contains(string_keys[i]);
            (void)found;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map<string> contains", duration.count(), N);
    }
    std::cout << "\n";

    // ===== STRING AT =====
    {
        std::unordered_map<std::string, size_t> um;
        for (size_t i = 0; i < N; i++) um[string_keys[i]] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            volatile size_t val = um.at(string_keys[i]);
            (void)val;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map<string> at", duration.count(), N);
    }
    {
        hash_map<std::string, size_t> hm;
        for (size_t i = 0; i < N; i++) hm[string_keys[i]] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            volatile auto val = hm.at(string_keys[i]);
            (void)val;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map<string> at", duration.count(), N);
    }
    std::cout << "\n";

    // ===== STRING ERASE =====
    {
        std::unordered_map<std::string, size_t> um;
        for (size_t i = 0; i < N; i++) um[string_keys[i]] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            um.erase(string_keys[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map<string> erase", duration.count(), N);
    }
    {
        hash_map<std::string, size_t> hm;
        for (size_t i = 0; i < N; i++) hm[string_keys[i]] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            hm.erase(string_keys[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map<string> erase", duration.count(), N);
    }

    return 0;
}
