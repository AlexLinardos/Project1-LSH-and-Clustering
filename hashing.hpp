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
    int w; // window
    int d; // vector dimensions

public:
    H(int window, int dimensions) : w(window), d(dimensions){};

    int produce_h(Item p)
    {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine eng(seed); // create a randomisation engine with time as seed

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

    vector<int> produce_k_h(Item p, int k)
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

        vector<int> h_vector;
        for (int i = 0; i < k; i++)
        {
            h_vector.push_back(floor(((inner_product(p.xij.begin(), p.xij.end(), &v.at(i), 0)) + t) / this->w));
        }
        // for(int j=0; j<4;j++)
        // {
        //     cout << h_vector[j] << ", ";
        // }
        return h_vector;
    }
};

class G
{
    int k;
    int tableSize;
    int w; // window
    int d; // vector dimensions

public:
    G(int k, int tableSize, int window, int dimensions) : k(k), tableSize(tableSize), w(window), d(dimensions){};

    int produce_g(Item p)
    {
        H test_H = H(w, d);
        vector<int> h_vector = test_H.produce_k_h(p, 4);
        for (int j = 0; j < 4; j++)
        {
            cout << h_vector[j] << ", ";
        }
        cout << endl;

        default_random_engine eng{static_cast<long unsigned int>(time(0))}; // create a randomisation engine with time as seed

        vector<float> r;
        normal_distribution<float> nd(0, 1); // create a normal distribution N(0,1)

        // create a vector v whose points follow the normal distribution
        for (int i = 0; i < k; i++)
        {
            r.push_back(nd(eng));
        }

        long unsigned int m = (long unsigned int)(((long long)1 << 32) - (long long)5);
        cout << m << endl;
        return 0;
    }
};

#endif