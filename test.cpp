#include <iostream>
#include <string>
#include "hash_map.hpp"
using namespace std;

int main() {
    cout << "=== Test 1: Basic insert and at ===" << endl;
    hash_map<int, int> mp;
    mp.insert(1, 100);
    mp.insert(2, 200);
    mp.insert(3, 300);
    cout << "mp.at(1) = " << mp.at(1) << " (expected 100)" << endl;
    cout << "mp.at(2) = " << mp.at(2) << " (expected 200)" << endl;
    cout << "mp.at(3) = " << mp.at(3) << " (expected 300)" << endl;

    cout << "\n=== Test 2: operator[] read ===" << endl;
    cout << "mp[1] = " << mp[1] << " (expected 100)" << endl;
    cout << "mp[2] = " << mp[2] << " (expected 200)" << endl;

    cout << "\n=== Test 3: operator[] creates entry ===" << endl;
    mp[99] = 999;
    cout << "mp[99] = " << mp[99] << " (expected 999)" << endl;

    cout << "\n=== Test 4: Update existing key ===" << endl;
    mp.insert(1, 111);
    cout << "mp[1] = " << mp[1] << " (expected 111)" << endl;

    cout << "\n=== Test 5: String keys ===" << endl;
    hash_map<string, int> ages;
    ages.insert("alice", 30);
    ages.insert("bob", 25);
    ages["charlie"] = 35;
    cout << "ages[alice] = " << ages["alice"] << " (expected 30)" << endl;
    cout << "ages[bob] = " << ages["bob"] << " (expected 25)" << endl;
    cout << "ages[charlie] = " << ages["charlie"] << " (expected 35)" << endl;

    cout << "\n=== Test 6: Missing key ===" << endl;
    cout << "mp.at(9999) = " << mp.at(9999) << " (expected 0, default)" << endl;

    cout << "\nAll tests complete!" << endl;

    return 0;
}