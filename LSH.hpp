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

    long unsigned int produce_g(Item p)
    {
        H test_H = H(w, d, k);
        vector<int> h = test_H.produce_k_h(p);
        for (int j = 0; j < k; j++)
        {
            cout << h[j] << ", ";
        }
        cout << endl;

        vector<int> r;
        std::uniform_int_distribution<int> uid(0, w-1);

        // create a vector v whose points follow the uniform real distribution
        for (int i = 0; i < k; i++)
        {
            r.push_back(uid(eng));
        }

        long unsigned int sum=0;
        for(int i=0; i<k; i++)
        {
            sum = sum + (((long unsigned)r[i]*(long unsigned)h[i])%(m))%((long unsigned)tableSize);
        }

        return sum;
    }
};