#ifndef HASHING_HPP
#define HASHING_HPP
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <numeric>
#include "utilities.hpp"

using namespace std;

// hash function family H
class H
{
    int w; // window
    int d; // vector dimensions

public:
    H(int window, int dimensions) : w(window), d(dimensions){};

    int produce_h(Item p)
    {
        default_random_engine eng{static_cast<long unsigned int>(time(0))}; // create a randomisation engine with time as seed

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
};

#endif