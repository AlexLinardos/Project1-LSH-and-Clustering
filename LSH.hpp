#ifndef LSH_HPP
#define LSH_HPP
#include "hashing.hpp"
#include "LSH_ui.hpp"
#include <algorithm>
#include <cmath>

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

        // sum = sum % (long unsigned)tableSize;
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
    LSH(LSH_params params, int factor_for_windowSize, int divisor_for_tableSize): params(params)
    {
        // tune windowSize
        dataset = read_items(params.input_file);
        queries = read_items(params.query_file);
        tableSize = dataset.size()/divisor_for_tableSize;
        dimension = dataset[0].xij.size();

        std::random_device rd;     // only used once to initialise (seed) engine
        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_int_distribution<int> uni(0,dataset.size()-1); // guaranteed unbiased
        int item_index_1;
        int item_index_2;
        double distance=0;
        
        for(int i=0; i<dataset.size()/4; i++)
        {
            item_index_1 = uni(rng);
            item_index_2 = uni(rng);
            while(item_index_1==item_index_2)
                item_index_2 = uni(rng);
            distance += (EuclideanDistance(&dataset[item_index_1], &dataset[item_index_2], dimension))/(double)(dataset.size()/4);
        }

        windowSize = factor_for_windowSize*(int)distance;
        // cout << "w " << w << endl;

        // Initialize L hashTables and g_hashFunctions
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
                unsigned int bucket = g[i]->produce_g(dataset[a]) % (long unsigned)tableSize;
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
        // initialize a vector of N best candidates and distances represented as c++ pairs
        std::vector<std::pair<int, Item*>> knns;
        // Then initialize each pair with distance -> (max integer) and a null item
        for (int i = 0; i < N; i++)
        {
            Item item = Item();
            knns.push_back(std::make_pair(std::numeric_limits<int>::max(), &item));
        }

        // For each hash table...
        int itemsSearched = 0;
        for (int i = 0; i < params.L; i++) 
        {
            // Calculate the bucket to which the query item corresponds
            long unsigned id = g[i]->produce_g(*query);
            long unsigned bucket = id % (long unsigned)tableSize;

            // For each item inside the bucket...
            for (int j = 0; j < hashTables[i][bucket].size(); j++) 
            {
                /*
                Check if the current item is already inserted into the KNNs vector from a previous hash table.
                */
                bool alreadyExists = false;
                for (int a = 0; a < N; a++)
                    if (knns[a].second->id == hashTables[i][bucket][j]->id)
                    alreadyExists = true;

                if (alreadyExists)
                    continue;

                // // // Querying trick (from Lecture Slides): ID is locality sensitive. Avoid computing Euclidean distance for all elements in bucket.
                // if(g[i]->produce_g(*(hashTables[i][bucket][j]))!=id)
                //     continue;

                // Calculate item's distance to the query item
                int distance = EuclideanDistance(query, hashTables[i][bucket][j], dimension);

                /*
                The last pair in the N-sized vector is the worst out of the N
                best candidates till now. If a better candidate is found,
                replace the last pair with the new one and re-sort the vector.
                */
                if (distance < knns[N-1].first) {
                    knns[N-1].first = distance;
                    // if (knns[N-1].second->null) // if it is a null item created just to initialize the N pairs of the vector.
                    //     delete knns[N-1].second;
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

    /*
    Each neighbor is represented as a pair of <distanceToQuery, neighborItem*>
    The following function returns a vector of these pairs
    */
    std::vector<std::pair<int, Item*>> RangeSearch (Item* query, double radius, int thresh = 0) {
        // Initialize the vector
        std::vector<std::pair<int, Item*>> d;
        /*
        In this method, we do not need to sort the vector, also its size is not constant.
        Hence, we do not need to initalize its values.
        Simply, whenever a neighbor has distance less than radius, we add it to the vector
        */

        // For each hash table...
        int itemsSearched = 0;
        for (int i = 0; i < params.L; i++) {
        // Calculate the bucket to which the query item corresponds
            long unsigned id = g[i]->produce_g(*query);
            long unsigned bucket = id % (long unsigned)tableSize;

            // For each item inside the bucket...
            for (int j = 0; j < hashTables[i][bucket].size(); j++) {
                // Check if the current item is already inserted into the vector
                bool alreadyExists = false;
                for (int a = 0; a < d.size(); a++)
                if (d[a].second->id == hashTables[i][bucket][j]->id)
                    alreadyExists = true;

                /*
                The "marked" condition will only be met whenever this function is used by
                reverse assignment in clustering. When LSH is used for ANN, it will have no effect.
                In reverse assignment, to avoid fetching the same items, we "mark" them when inserted
                to a cluster so as to indicate that they are already assigned.
                */
                if (alreadyExists /*|| hashTables[i][bucket][j]->marked*/)
                continue;

                int distance = EuclideanDistance(query, hashTables[i][bucket][j], dimension);

                // If the distance is less than radius, insert the pair into the return vector
                if (distance < radius) {
                std::pair<int, Item*> tmpPair = std::make_pair(distance, hashTables[i][bucket][j]);
                d.push_back(tmpPair);
                }

                // If a certain threshold of items traversed is reached, return the vector.
                if (thresh != 0 && ++itemsSearched >= thresh)
                {
                    std::sort(d.begin(), d.end(), comparePairs);
                    return d;
                }
            }
        }
        // std::sort(d.begin(), d.end(), comparePairs);
        return d;
    }

    std::vector<std::pair<int, Item*>> brute_force_search (Item* query, int N) {
        // initialize a vector of N best candidates and distances represented as c++ pairs
        std::vector<std::pair<int, Item*>> knns;
        // Then initialize each pair with distance -> (max integer) and a null item
        for (int i = 0; i < N; i++)
            knns.push_back(std::make_pair(std::numeric_limits<int>::max(), new Item()));

        // For each item in dataset
        for (int j = 0; j < dataset.size(); j++) 
        {
            // Calculate item's distance to the query item
            int distance = EuclideanDistance(query, &dataset[j], dimension);

            /*
            The last pair in the N-sized vector is the worst out of the N
            best candidates till now. If a better candidate is found,
            replace the last pair with the new one and re-sort the vector.
            */
            if (distance < knns[N-1].first) {
                knns[N-1].first = distance;
                if (knns[N-1].second->null && knns[N-1].second->id=="-1") // if it is a null item created just to initialize the N pairs of the vector.
                    delete knns[N-1].second;
                knns[N-1].second = &dataset[j];
                std::sort(knns.begin(), knns.end(), comparePairs);
            }
        }
        return knns;
    }
};

#endif