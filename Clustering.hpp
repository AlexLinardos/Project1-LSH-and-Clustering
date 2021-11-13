#ifndef CLUSTERING_HPP
#define CLUSTERING_HPP

#include "Clustering_ui.hpp"

#include <random>

class ClusterCenter
{
    vector<int> position;

public:
    ClusterCenter(vector<int> position) : position(position){

                                          };
};

class Clustering
{
    int dimension; // Data dimension

    Cluster_params params;

    vector<Item> dataset;
    vector<ClusterCenter> clusterCenters;

public:
    Clustering(Cluster_params params) : params(params)
    {
        read_items(dataset, params.input_file);
        dimension = dataset[0].xij.size();
    };

    vector<ClusterCenter> centers_init() // using k-means++
    {
        // 1. Choose first cluster center uniformly at random from data points
        std::random_device rd;                                         // only used once to initialise (seed) engine
        std::mt19937 rng(rd());                                        // random-number engine used (Mersenne-Twister in this case)
        std::uniform_int_distribution<int> uni(0, dataset.size() - 1); // guaranteed unbiased

        int item_index = uni(rng);

        // ClusterCenter *cc = new ClusterCenter(dataset[item_index].xij);
        // clusterCenters.push_back(*cc);
    }
};

#endif