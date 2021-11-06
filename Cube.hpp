#ifndef CUBE_HPP
#define CUBE_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include "hashing.hpp"
#include "Cube_ui.hpp"

using namespace std;

class F
{
public:
    int k; // Διάσταση στην οποία προβάλλονται τα σημεία
    map<int, int> mp;
    default_random_engine eng;
    uniform_int_distribution<int> uid;

    F(int k) : k(k), eng(chrono::system_clock::now().time_since_epoch().count()), uid(0, 1){};

    int produce_f(int h)
    {
        if (mp.count(h) == 0)
        {
            mp[h] = uid(eng);
            cout << h << ":" << mp[h] << endl;
        }
    }
};

#endif