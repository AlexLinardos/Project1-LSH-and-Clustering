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
        vector<Item> centers; // cluster centers initialized in initialize_pp()
        vector<vector<Item>> clusters;
        vector<int> assignments; // shows the cluster to which each point is assigned to

        Clustering(Cluster_params params, vector<Item> dataset) : params(params), dataset(dataset), eng(chrono::system_clock::now().time_since_epoch().count()), uid(0, dataset.size() - 1), assignments(dataset.size()) {}

        // initialization ++
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
                // cout << "SUM (after normalization) = " << dist_sum << endl;

                calculate_probs(d, dist_sum, p); // calculate probability of each point to be chosen as next centroid
                double sum_prob = 0.0;
                for (int k = 0; k < p.size(); ++k)
                {
                    sum_prob += p[k];
                }
                // cout << "THIS MUST BE 1.0 : " << sum_prob << endl;

                std::discrete_distribution<> distribution{p.begin(), p.end()};
                int pick = distribution(eng);
                centroids.push_back(dataset[pick]);
                // cout << "New centroid is point with id: " << dataset[pick].id << " and it had a probabillity of " << p[pick] << endl;
                double max_p = 0.0;
                for (int z = 0; z < p.size(); ++z)
                {
                    if (p[z] > max_p)
                    {
                        max_p = p[z];
                    }
                }
                // cout << "Max probabillity was " << max_p << endl
                //      << endl;
            }
            // cout << "CENTROIDS CREATED = " << centroids.size() << endl;
            this->centers = centroids;
            this->clusters.resize(centroids.size());
        }

        // calculate distance of each point from its nearest centroid
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
            // cout << "MAX = " << max_d << endl;

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

        // calculate probability of each point to be picked as next centroid
        void calculate_probs(vector<double> d, double dist_sum, vector<double> &p)
        {
            for (int i = 0; i < d.size(); ++i)
            {
                p[i] = pow(d[i], 2.0) / dist_sum;
            }
        }

        // assign a nearest center to each point (part 1 of Lloyd's algorithm)
        void assign_centers()
        {
            // cout << "ASSIGNING" << endl;
            int dimension = this->dataset[0].xij.size();
            for (int i = 0; i < this->dataset.size(); ++i)
            {
                double min_d = EuclideanDistance(&centers[0], &this->dataset[i], dimension);
                int nearest_cntr = 0;
                for (int c = 1; c < centers.size(); ++c)
                {
                    double next_d = EuclideanDistance(&centers[c], &this->dataset[i], dimension);
                    if (next_d < min_d)
                    {
                        min_d = next_d;
                        nearest_cntr = c;
                    }
                }
                this->assignments[i] = nearest_cntr;
                this->clusters[nearest_cntr].push_back(this->dataset[i]); // push it into a cluster based on assigned center
            }
        }

        // update centers (part 2 of Lloyd's algorithm)
        void update_centers()
        {
            int v_dimension = this->dataset[0].xij.size();
            // now we must calculate mean per cluster and make it the new center
            for (int i = 0; i < this->centers.size(); ++i)
            {
                vector<double> mean(v_dimension, 0.0);
                int T = this->clusters[i].size(); // number of objects in cluster

                // calculate mean
                for (int j = 0; j < T; ++j)
                {
                    mean = vector_mean(mean, this->clusters[i][j].xij, v_dimension, T); // using vector_addition from utilities.hpp
                }

                // // cout << "Sample mean: ";
                // for (int m = 0; m < 20; ++m)
                // {
                //     cout << mean[m] << " ";
                // }
                // cout << endl;

                this->centers[i].xij = mean;
            }
            // cout << "New centers assigned" << endl;
            // cout << ".........................................." << endl;
        }

        // Lloyd's classic clustering algorithm
        void Lloyds(int max_iter)
        {
            int iter = 1; // iterations
            assign_centers();
            update_centers();

            vector<int> last_assignments(this->assignments.size());
            do
            {
                for (int i = 0; i < this->clusters.size(); ++i)
                {
                    this->clusters[i].clear(); // clear clusters so we can reassign the items
                }
                last_assignments = this->assignments;
                assign_centers();
                update_centers();
                iter++;
                // iterate until assignments don't change or until we reach max_iter threshold
            } while ((!equal(assignments.begin(), assignments.end(), last_assignments.begin())) && iter < max_iter);
            cout << "............................................" << endl;
            cout << "Lloyd's algorithm ended after " << iter << " iterations" << endl;
        }

        // Silhouette of object at index i
        double silhouette(int i)
        {
            vector<Item> *cluster = &(this->clusters[this->assignments[i]]); // find out at which cluster this item is assigned to

            //  calculate a(i) = average distance of i to objects in same cluster
            vector<double> distances;
            int dimension = (*cluster)[0].xij.size();

            for (int j = 0; j < (*cluster).size(); ++j)
            {
                double dist = EuclideanDistance(&this->dataset[i], &((*cluster)[j]), dimension);
                distances.push_back(dist);
            }

            double a = 0.0;
            for (int j = 0; j < distances.size(); ++j)
            {
                a += distances[j] / (double)distances.size();
            }

            // calculate b(i) = average distance of i to objects in next best (neighbor) cluster, i.e. cluster of 2nd closest centroid
            // first find the index of the next best cluster
            int best;
            double best_dist = std::numeric_limits<double>::max();
            for (int j = 0; j < this->centers.size(); ++j)
            {
                if (j != this->assignments[i])
                {
                    double dist = EuclideanDistance(&this->dataset[i], &this->centers[j], dimension);
                    if (dist < best_dist)
                    {
                        best_dist = dist;
                        best = j;
                    }
                }
            }

            // now perform the calculations
            distances.clear();
            cluster = &(this->clusters[best]);
            for (int j = 0; j < (*cluster).size(); ++j)
            {
                double dist = EuclideanDistance(&this->dataset[i], &((*cluster)[j]), dimension);
                distances.push_back(dist);
            }
            double b = 0.0;
            for (int j = 0; j < distances.size(); ++j)
            {
                b += distances[j] / (double)distances.size();
            }
            // find max between a(i) and b(i)
            double max = a;
            if (b > a)
            {
                max = b;
            }
            // finally calculate and return silhouette
            return (b - a) / max;
        }

        // metric to evaluate specific cluster (c_index = the index of said cluster)
        double eval_specific_cluster(int c_index)
        {
            // cout << "Evaluating cluster: " << c_index << endl;
            double average = 0.0;
            double s = 0.0;
            int n = this->clusters[c_index].size();
            for (int i = 0; i < this->dataset.size(); ++i)
            {
                if (this->assignments[i] == c_index)
                {
                    s = silhouette(i);
                    average += s / (double)n;
                }
            }
            return s;
        }

        // metric to evaluate overall clustering
        double eval_clustering()
        {
            double average = 0.0;
            double s = 0.0;
            int n = this->dataset.size();
            for (int i = 0; i < n; ++i)
            {
                s = silhouette(i);
                average += s / (double)n;
            }
            return s;
        }
    };
}

#endif