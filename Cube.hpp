#ifndef CUBE_HPP
#define CUBE_HPP
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <typeinfo>
#include "hashing.hpp"
#include "Cube_ui.hpp"
#include "utilities.hpp"

using namespace std;

class F
{
public:
    int k; // Διάσταση στην οποία προβάλλονται τα σημεία
    vector<unordered_map<int, int>> h_maps;
    default_random_engine eng;
    uniform_int_distribution<int> uid;

    F(int k) : k(k), h_maps(k), eng(chrono::system_clock::now().time_since_epoch().count()), uid(0, 1){};

    int produce_f(int index, int h)
    {
        if (h_maps[index].count(h) == 0)
        {
            int rnum = uid(eng);
            h_maps[index][h] = rnum;
            // cout << "h" << index << ":" << h << "-> " << h_maps[index][h] << endl;
            return rnum;
        }
        else
        {
            return h_maps[index].find(h)->second;
        }
    }
};

class Hypercube
{
public:
    int k;                 // Διάσταση στην οποία προβάλλονται τα σημεία
    int M;                 // Μέγιστο πλήθος υποψήφιων σημείων που θα ελεγχθούν
    int probes;            // Μέγιστο επιτρεπόμενο πλήθος κορυφών που θα ελεγχθούν
    int N;                 // Αριθμός πλησιέστερων γειτόνων
    double R;              // Ακτίνα αναζήτησης
    int d;                 // Διαστάσεις αντικειμένων
    int w;                 // Window
    unsigned int vertices; // Hypercube vertices
    vector<Item> dataset;
    vector<unordered_map<int, int>> h_maps;
    vector<vector<Item>> hash_table;
    F f;

    Hypercube(Cube_params params, int d, int w, vector<Item> dataset, vector<unordered_map<int, int>> h_maps) : d(d), w(w), dataset(dataset), h_maps(h_maps), f(params.k)
    {
        this->k = params.k;
        this->M = params.M;
        this->probes = params.probes;
        this->N = params.N;
        this->R = params.R;
        this->vertices = pow(2, k);

        hash_table = vector<vector<Item>>(vertices);
        cout << "INITIALIZED HASH TABLE OF SIZE: " << hash_table.size() << endl;
        for (int i = 0; i < vertices; ++i)
        {
            vector<Item> dummy_vec;
            hash_table[i] = dummy_vec;
        }

        for (int i = 0; i < dataset.size(); ++i)
        {
            H h(w, d, k);
            vector<int> h_functions = h.produce_k_h(dataset[i]);
            vector<int> f_values(k);
            for (int j = 0; j < k; ++j)
            {
                f_values.push_back(f.produce_f(j, h_functions[j]));
            }
            unsigned int bucket = concat_f_values(f_values);
            hash_table[bucket].push_back(dataset[i]);
        }

        for (int i = 0; i < 20; ++i)
        {

            for (int j = 0; j < hash_table[i].size(); ++j)
            {
                Item test = hash_table[i][j];
            }
        }
    }

    ~Hypercube() {}

    unsigned int find_bucket(Item q)
    {
        H h(w, d, k);
        vector<int> h_functions = h.produce_k_h(q);
        vector<int> f_values(k);
        for (int j = 0; j < k; ++j)
        {
            f_values.push_back(f.produce_f(j, h_functions[j]));
        }
        return concat_f_values(f_values);
    }

    unsigned int concat_f_values(vector<int> f_values)
    {
        unsigned int num = 0;
        for (int i = 0; i < f_values.size(); ++i)
        {
            if (f_values[i] == 1)
            {
                num |= 1;
            }

            if (i < (f_values.size() - 1))
            {
                num <<= 1;
            }
        }
        return num;
    }

    void print_buckets(int num)
    {
        for (int i = 0; i < num; ++i)
        {
            cout << "Bucket " << i << " has " << hash_table[i].size() << " items." << endl;
        }
    }

    vector<int> get_probes_in_distance(int dist, unsigned int bucket)
    {
        vector<int> result;

        for (int i = 0; i < this->vertices; ++i)
        {
            if (HammingDistance(bucket, i) == dist)
            {
                result.push_back(i);
            }
        }
        return result;
    }

    vector<int> get_probes_in_threshold(unsigned int bucket)
    {
        vector<int> result;

        int probes_reached = 0;
        for (int i = 0; i < this->vertices; ++i)
        {
            vector<int> p_in_dist = get_probes_in_distance(i, bucket);
            cout << "DISTANCE" << i << ": ";
            for (int j = 0; j < p_in_dist.size(); ++j)
            {
                cout << p_in_dist[j] << " ";
            }
            cout << endl;

            for (int j = 0; j < p_in_dist.size(); ++j)
            {
                result.push_back(p_in_dist[j]);
                probes_reached++;
                if (probes_reached >= this->probes)
                {
                    cout << "Ended early at " << probes_reached << " probes." << endl;
                    return result;
                }
            }
        }
        cout << "I reached " << probes_reached << " instead of " << this->probes << endl;
        return result;
    }

    std::vector<std::pair<int, Item *>> kNN(Item query)
    {
        // At first initalize the result vector of <distanceFromQuery, item> pairs
        std::vector<std::pair<int, Item *>> knns;
        // Then initialize each pair with distance -> (max integer) and a null item
        for (int i = 0; i < this->N; i++)
            knns.push_back(std::make_pair(std::numeric_limits<int>::max(), new Item()));

        unsigned int q_bucket = find_bucket(query);
        cout << "Query " << query.id << " is in bucket " << q_bucket << endl;
        vector<int> search_probes = get_probes_in_threshold(q_bucket);

        cout << "-----------------------------------------------------" << endl;
        int items_searched = 0;
        for (int i = 0; i < search_probes.size(); ++i)
        {
            int curr_bucket = search_probes[i];
            cout << "Searching bucket " << curr_bucket << " with " << hash_table[curr_bucket].size() << " items inside." << endl;
            cout << "BUCKET " << curr_bucket << ": ";
            for (int j = 0; j < hash_table[curr_bucket].size(); ++j)
            {
                cout << hash_table[curr_bucket][j].id << ", ";
            }
            cout << endl;

            for (int j = 0; j < hash_table[curr_bucket].size(); ++j)
            {
                cout << "Item " << hash_table[curr_bucket][j].id << " in ";
                int dist = EuclideanDistance(&query, &hash_table[curr_bucket][j], d);
                cout << "distance " << dist << " | ";

                if (dist < knns[N - 1].first)
                {
                    knns[N - 1].first = dist;
                    if (knns[N - 1].second->null && knns[N - 1].second->id == "-1") // if it is a null item created just to initialize the N pairs of the vector.
                        delete knns[N - 1].second;
                    knns[N - 1].second = &hash_table[curr_bucket][j];
                    std::sort(knns.begin(), knns.end(), comparePairs);
                }

                items_searched++;
                if (items_searched >= this->M)
                {
                    return knns;
                }
            }
            cout << endl;
            cout << "...................." << endl;
        }
        return knns;
    }

    std::vector<std::pair<int, Item *>> RangeSearch(Item query)
    {
        // At first initalize the result vector of <distanceFromQuery, item> pairs
        std::vector<std::pair<int, Item *>> rns;
        unsigned int q_bucket = find_bucket(query);
        cout << "Query " << query.id << " is in bucket " << q_bucket << endl;
        vector<int> search_probes = get_probes_in_threshold(q_bucket);

        cout << "-----------------------------------------------------" << endl;
        int items_searched = 0;
        for (int i = 0; i < search_probes.size(); ++i)
        {
            int curr_bucket = search_probes[i];
            cout << "Searching bucket " << curr_bucket << " with " << hash_table[curr_bucket].size() << " items inside." << endl;
            cout << "BUCKET " << curr_bucket << ": ";
            for (int j = 0; j < hash_table[curr_bucket].size(); ++j)
            {
                cout << hash_table[curr_bucket][j].id << ", ";
            }
            cout << endl;

            for (int j = 0; j < hash_table[curr_bucket].size(); ++j)
            {
                // Εδώ κάνεις κάποια plays με ένα alreadyExists που δεν έχω ιδεά τι είναι
                // Το αφήνω να το βάλεις εσύ καλύτερα

                cout << "Item " << hash_table[curr_bucket][j].id << " in ";
                int dist = EuclideanDistance(&query, &hash_table[curr_bucket][j], d);
                cout << "distance " << dist << " | ";

                if (dist < this->R)
                {
                    std::pair<int, Item *> tmp_pair = std::make_pair(dist, &hash_table[curr_bucket][j]);
                    rns.push_back(tmp_pair);
                }

                items_searched++;
                if (items_searched >= this->M)
                {
                    return rns;
                }
            }
            cout << endl;
            cout << "...................." << endl;
        }
        return rns;
    }
};

#endif