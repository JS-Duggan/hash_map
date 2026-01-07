#include <iostream>
#include <memory>
#include "hash_map.hpp"
using namespace std;
int main() {
    hash_map<int, int> mp;
    mp.insert(4, 3);
    cout << mp[4] << endl;
}