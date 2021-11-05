#ifndef HASHING_HPP
#define HASHING_HPP
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <numeric>
#include <chrono>
#include "utilities.hpp"

using namespace std;

// hash function family H
class H
{
    int k; // number of H functions used in the algorithm
    int w; // window
    int d; // vector dimensions
    default_random_engine eng;

public:
    H(int window, int dimensions, int k) : w(window), d(dimensions), k(k)
    {
        unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine eng(time(0)+clock()); // create a randomisation engine with time as seed
    };

    int produce_h(Item p)
    {
        vector<float> v;
        normal_distribution<float> nd(0, 1); // create a normal distribution N(0,1)

        // create a vector v whose points follow the normal distribution
        for (int i = 0; i < d; i++)
        {
            v.push_back(nd(eng));
        }

        uniform_real_distribution<double> urd(0, w); // create a uniform real distribution in [0,w)
        float t = urd(eng);

        return floor(((inner_product(p.xij.begin(), p.xij.end(), v.begin(), 0)) + t) / this->w);
    }

    vector<int> produce_k_h(Item p)
    {
        vector<int> h_vector;
        for (int i = 0; i < k; i++)
        {
            h_vector.push_back(produce_h(p));
        }
        return h_vector;
    }
};

#endif