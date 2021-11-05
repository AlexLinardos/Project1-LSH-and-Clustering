#include "hashing.hpp"

class G
{
    int k; // number of H functions used in the algorithm
    int tableSize;
    int w; // windowSize inserted by the user
    int d; // dimension of vector
    long unsigned int m;
    default_random_engine eng;

public:
    G(int k, int tableSize, int window, int dimensions) : k(k), tableSize(tableSize), w(window), d(dimensions)
    {
        m = (long unsigned int)(((long long)1 << 32) - (long long)5);
        default_random_engine eng(time(0)+clock()); // create a randomisation engine with time as seed
    };

    int produce_g(Item p)
    {
        H test_H = H(w, d, k);
        vector<int> h_vector = test_H.produce_k_h(p);
        for (int j = 0; j < k; j++)
        {
            cout << h_vector[j] << ", ";
        }
        cout << endl;

        vector<float> r;
        uniform_real_distribution<double> urd(0, w); // create a uniform real distribution in [0,w)

        // create a vector v whose points follow the uniform real distribution
        for (int i = 0; i < k; i++)
        {
            r.push_back(urd(eng));
        }

        return 0;
    }
};