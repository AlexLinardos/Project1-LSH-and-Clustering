#ifndef CLUSTERING2_HPP
#define CLUSTERING2_HPP
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include "Clustering_ui.hpp"

using namespace std;

namespace Alekos
{
    class Clustering
    {
    public:
        Cluster_params params;
        vector<Item> dataset;
        default_random_engine eng;
        uniform_int_distribution<int> uid;

        Clustering(Cluster_params params, vector<Item> dataset) : params(params), dataset(dataset), eng(chrono::system_clock::now().time_since_epoch().count()), uid(0, dataset.size() - 1) {}

        void initialize_pp()
        {
            vector<Item> centroids;

            // pick first centroid at random
            int rcentroid_index = uid(eng);
            centroids.push_back(dataset[rcentroid_index]);

            // continue by picking more centroids until we reach the given number of clusters
            // centroids must be far away from each other
            while (centroids.size() < this->params.clusters)
            {
                double dist_sum = 0.0;
                vector<double> d(dataset.size(), 0.0); // distances of points to closest centroid
                vector<double> p(dataset.size(), 0.0); // probabilities of points to be chosen as next centroid

                calculate_min_dists(centroids, d, dist_sum); // calculate min distance of all points to the closest centroid for them
                cout << "SUM (after normalization) = " << dist_sum << endl;

                calculate_probs(d, dist_sum, p); // calculate probability of each point to be chosen as next centroid
                double sum_prob = 0.0;
                for (int k = 0; k < p.size(); ++k)
                {
                    sum_prob += p[k];
                }
                cout << "THIS MUST BE 1.0 : " << sum_prob << endl;
                break;
                //
                // here goes the code to assign next centroid (instead of break)
                //
            }
            cout << "CENTROIDS CREATED= " << centroids.size() << endl;
        }

        void calculate_min_dists(vector<Item> &centroids, vector<double> &d, double &dist_sum)
        {
            int dimension = this->dataset[0].xij.size();
            double max_d = 0.0; // for normalization

            for (int i = 0; i < this->dataset.size(); ++i)
            {
                double min_dist = EuclideanDistance(&centroids[0], &this->dataset[i], dimension);
                if (min_dist > max_d)
                {
                    max_d = min_dist;
                }

                for (int cntr = 1; cntr < centroids.size(); ++cntr)
                {
                    double dist = EuclideanDistance(&centroids[cntr], &this->dataset[i], dimension);
                    if (dist > max_d)
                    {
                        max_d = dist;
                    }
                    if (dist < min_dist)
                    {
                        min_dist = dist;
                    }
                }
                d[i] = min_dist;
            }
            cout << "MAX = " << max_d << endl;

            // normalize data
            for (int j = 0; j < d.size(); ++j)
            {
                d[j] = d[j] / max_d;
            }

            // calculate sum ΣD(i)^2
            for (int i = 0; i < d.size(); ++i)
            {
                dist_sum += pow(d[i], 2.0);
            }
        }

        void calculate_probs(vector<double> d, double dist_sum, vector<double> &p)
        {
            for (int i = 0; i < d.size(); ++i)
            {
                p[i] = d[i] / dist_sum;
            }
            cout << endl;
        }
    };
}

#endif