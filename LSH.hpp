#ifndef LSH_HPP
#define LSH_HPP
#include "hashing.hpp"
#include <algorithm>

class G
{
    int k; // number of H functions used in the algorithm
    int tableSize;
    int w; // windowSize inserted by the user
    int d; // dimension of vector
    long unsigned int m;
    default_random_engine eng;
    uniform_int_distribution<int> uid;

public:
    G(int k, int tableSize, int window, int dimensions) : k(k), tableSize(tableSize), w(window), d(dimensions), eng(time(0) + clock()), uid(0, w - 1)
    {
        m = (long unsigned int)(((long long)1 << 32) - (long long)5);
    };

    unsigned int produce_g(Item p)
    {
        H h_familly = H(w, d, k);
        vector<int> h = h_familly.produce_k_h(p);
        // for (int j = 0; j < k; j++)
        // {
        //     cout << h[j] << ", ";
        // }
        // cout << endl;

        vector<int> r;

        // create a vector v whose points follow the uniform real distribution
        for (int i = 0; i < k; i++)
        {
            r.push_back(uid(eng));
        }

        long unsigned int sum = 0;
        for (int i = 0; i < k; i++)
        {
            /*
            We are using the following formula to not achieve overflow:
            (a$b)modM = ((amodM)$(bmodM))modM.
            $ represents {+, -, *, /}
            */
            // Calculate a mod M
            long unsigned int amodm = (long unsigned)r[i] % m;

            // Calculate b mod M
            long unsigned int bmodm = (long unsigned)h[i] % m;

            // Calculate (amodM*bmodM)modM
            long unsigned int resmodm = (amodm * bmodm) % m;

            // Add the above into the sum
            sum += resmodm;
        }

        sum = sum % m;
        // p.id = sum;

        sum = sum % (long unsigned)tableSize;
        return (unsigned int)sum;
    }
};

class LSH
{
    int dimension; // Data dimension
    
    int windowSize; // windowSize
    
    int tableSize; // hashtableSize

    LSH_params params; // k, L, N, R

    vector<Item> dataset;
    vector<Item> queries;

    std::vector<Item*>** hashTables;
    G** g;

public:
    LSH(LSH_params params, int windowSize, int divisor_for_tableSize): windowSize(windowSize), params(params)
    {
        dataset = read_items(params.input_file);
        queries = read_items(params.query_file);
        tableSize = dataset.size()/divisor_for_tableSize;
        dimension = dataset[0].xij.size();

        // Initialize L hash tables and g_hashFunctions
        hashTables= new std::vector<Item*>*[params.L];
        g = new G*[params.L];
        for (int i = 0; i < params.L; i++) // for every hashTable
        {
            hashTables[i]=new std::vector<Item*>[tableSize];
            g[i] = new G(params.k, tableSize, windowSize, dimension);
            // cout << g[i]->produce_g(dataset[0]) << " ";
        }
        // cout << endl;

        // Hash all items in training set and insert them into their buckets
        for (int a = 0; a < dataset.size(); a++) {
            for (int i = 0; i < params.L; i++) {
                unsigned int bucket = g[i]->produce_g(dataset[a]);
                hashTables[i][bucket].push_back(&dataset[a]);
            }
        }
    }
    ~LSH () 
    {
        for (int i = 0; i < params.L; i++) // for every hashTable
        {
            delete[] hashTables[i];
            delete g[i];
        }
        delete[] hashTables;
        delete[] g;
    }

    std::vector<std::pair<int, Item*>> kNN (Item* query, int N, int thresh = 0) {
        // At first initalize the vector itself
        std::vector<std::pair<int, Item*>> knns;
        // Then initialize each pair with distance -> (max integer) and a null item
        for (int i = 0; i < N; i++)
            knns.push_back(std::make_pair(std::numeric_limits<int>::max(), new Item()));

        // For each hash table...
        int itemsSearched = 0;
        for (int i = 0; i < params.L; i++) {
            // Calculate the bucket to which the query item corresponds
            int bucket = g[i]->produce_g(queries[0]);

            // For each item inside the bucket...
            for (int j = 0; j < hashTables[i][bucket].size(); j++) {
            /*
            Check if the current item is already inserted into the KNNs vector
            (it is possible since it may have been inserted from a previous hash table loop)
            */
            bool alreadyExists = false;
            for (int a = 0; a < N; a++)
                if (knns[a].second->id == hashTables[i][bucket][j]->id)
                alreadyExists = true;

            if (alreadyExists)
                continue;

            // Since it does not exist in the vector, calculate its distance to the query item
            int distance = EuclideanDistance(&queries[0], hashTables[i][bucket][j], dimension);

            /*
            If the distance is less than the last pair's in the vector,
            replace the pair with the new distance and the current item.
            Then, sort the vector by ascending order based on distance.
            This is done so that whenever we find a good neighbor candidate,
            we replace the least similar neighbor in the vector
            */
            if (distance < knns[N-1].first) {
                knns[N-1].first = distance;
                if (knns[N-1].second->null && knns[N-1].second->id=="-1") // if it is a null item created just to initialize the N pairs
                    delete knns[N-1].second;
                knns[N-1].second = hashTables[i][bucket][j];
                std::sort(knns.begin(), knns.end(), comparePairs);
            }

            /*
            If a certain threshold of items traversed is reached, return the vector.
            If thresh == 0 it indicates that the user does not want to add a threshold.
            */
            if (thresh != 0 && ++itemsSearched >= thresh)
                return knns;
            }
        }
        return knns;
    }
};

#endif