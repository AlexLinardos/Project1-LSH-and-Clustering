#include "hashing.hpp"

class G
{
    int k; // number of H functions used in the algorithm
    int tableSize;
    int w; // windowSize inserted by the user
    int d; // dimension of vector

public:
    G(int k, int tableSize, int window, int dimensions) : k(k), tableSize(tableSize), w(window), d(dimensions){};

    // int produce_g(Item p)
    // {
    //     H test_H = H(w, d, 4);
    //     vector<int> h_vector = test_H.produce_k_h(p, 4);
    //     for (int j = 0; j < 4; j++)
    //     {
    //         cout << h_vector[j] << ", ";
    //     }
    //     cout << endl;

    //     default_random_engine eng{static_cast<long unsigned int>(time(0))}; // create a randomisation engine with time as seed

    //     vector<float> r;
    //     normal_distribution<float> nd(0, 1); // create a normal distribution N(0,1)

    //     // create a vector v whose points follow the normal distribution
    //     for (int i = 0; i < k; i++)
    //     {
    //         r.push_back(nd(eng));
    //     }

    //     long unsigned int m = (long unsigned int)(((long long)1 << 32) - (long long)5);
    //     cout << m << endl;
    //     return 0;
    // }
};