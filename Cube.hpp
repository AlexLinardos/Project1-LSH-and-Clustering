#ifndef CUBE_HPP
#define CUBE_HPP
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
#include "hashing.hpp"
#include "Cube_ui.hpp"
#include "utilities.hpp"

using namespace std;

class F
{
public:
    map<int, int> mp;
    default_random_engine eng;
    uniform_int_distribution<int> uid;

    F() : eng(chrono::system_clock::now().time_since_epoch().count()), uid(0, 1){};

    void produce_f(int h)
    {
        if (mp.count(h) == 0)
        {
            mp[h] = uid(eng);
            cout << h << ":" << mp[h] << endl;
        }
    }
};

class Hypercube
{
public:
    int k;                 // Διάσταση στην οποία προβάλλονται τα σημεία
    int M;                 // Μέγιστο πλήθος υποψήφιων σημέιων που θα ελεγχθούν
    int probes;            // Μέγιστο επιτρεπόμενο πλήθος κορυφών που θα ελεγχθούν
    int d;                 // Διαστάσεις αντικειμένων
    int w;                 // Window
    unsigned int vertices; // Hypercube vertices
    vector<Item> dataset;
    unordered_map<int, int> hash_table;

    Hypercube(Cube_params params, int d, int w, vector<Item> dataset) : d(d), w(w), dataset(dataset)
    {
        this->k = params.k;
        this->M = params.M;
        this->probes = params.probes;
        this->vertices = pow(2, k);
    }
};

#endif