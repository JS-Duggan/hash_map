#include <iostream>
#include <string>
#include "hash_map.hpp"
using namespace std;

int main() {
    hash_map<int, int> mp;
    for (int i = 0; i < 9; i++) mp[i] = i;
    cout << mp[6] <<  "size: " << mp.size() << endl;
    mp.erase(6);
    cout <<  "size: " << mp.size() << endl;

}