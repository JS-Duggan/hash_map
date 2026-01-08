#include <iostream>
#include <iomanip>
#include <chrono>
#include <unordered_map>
#include "hash_map.hpp"

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

    // ===== INSERT =====
    {
        std::unordered_map<size_t, size_t> um;
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            um.insert({i, i});
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map insert", duration.count(), N);
    }
    {
        hash_map<size_t, size_t> hm;
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            hm.insert(i, i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map insert", duration.count(), N);
    }
    std::cout << "\n";

    // ===== CONTAINS =====
    {
        std::unordered_map<size_t, size_t> um;
        for (size_t i = 0; i < N; i++) um[i] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            volatile bool found = um.count(i) > 0;
            (void)found;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map contains", duration.count(), N);
    }
    {
        hash_map<size_t, size_t> hm;
        for (size_t i = 0; i < N; i++) hm[i] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            volatile bool found = hm.contains(i);
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
        for (size_t i = 0; i < N; i++) um[i] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            volatile size_t val = um.at(i);
            (void)val;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map at", duration.count(), N);
    }
    {
        hash_map<size_t, size_t> hm;
        for (size_t i = 0; i < N; i++) hm[i] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            volatile size_t val = hm.at(i);
            (void)val;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map at", duration.count(), N);
    }
    std::cout << "\n";

    // ===== ERASE =====
    {
        std::unordered_map<size_t, size_t> um;
        for (size_t i = 0; i < N; i++) um[i] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            um.erase(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("unordered_map erase", duration.count(), N);
    }
    {
        hash_map<size_t, size_t> hm;
        for (size_t i = 0; i < N; i++) hm[i] = i;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < N; i++) {
            hm.erase(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        print_result("hash_map erase", duration.count(), N);
    }

    return 0;
}