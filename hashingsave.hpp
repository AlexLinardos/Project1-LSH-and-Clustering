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
    int k; // number of h functions used in the algorithm
    int w; // window
    int d; // vector dimensions
    default_random_engine eng;

    int h_counter;
    std::vector<float>** v_vectors;

public:
    H(int window, int dimensions, int k) : w(window), d(dimensions), k(k)
    {
        unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine eng(time(0)+clock()); // create a randomisation engine with time as seed
        normal_distribution<float> nd(0, 1); // create a normal distribution N(0,1)

        h_counter=0;
        v_vectors = new std::vector<float>*[k];
        for (int i = 0; i < k; i++)
        {
            v_vectors[i]=new std::vector<float>[d];
            for (int j = 0; j < d; j++)
            {
                v_vectors[i]->push_back(nd(eng));
            }
        }
    };

    ~H () {
        for (int i = 0; i < k; i++)
        delete[] v_vectors[i];
        delete[] v_vectors;
    }

    int produce_h(Item p)
    {
        uniform_real_distribution<double> urd(0, w); // create a uniform real distribution in [0,w)
        float t = urd(eng);

        int result = floor(((inner_product(p.xij.begin(), p.xij.end(), v_vectors[h_counter]->begin(), 0)) + t) / this->w);
        h_counter+=1;
        return result;
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