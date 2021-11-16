#include <iostream>
#include <vector>
#include "../misc/utilities.hpp"
#include "CLUST.hpp"
#include <map>
#include <random>
#include <chrono>
using namespace std;

int main(int argc, char *argv[])
{
    Cluster_params params;
    if (argc == 1)
    {
        cout << "Run program from command line following the format below: " << endl;
        cout << "$./cluster -i <input file> -c <configuration file> -o <output file> -complete <optional> -m <method: Classic OR LSH or Hypercube>" << endl;
        return -1;
    }
    else
    {
        params = CMD_Interface(argc, argv);
        if (configuration(params) != 0)
        {
            return -1;
        }
        print_Cluster_params(params);
    }

    vector<Item> dataset;
    read_items(dataset, params.input_file);
    Clustering *clustering = new Clustering(params, dataset);
    int dimension = dataset[0].xij.size();

    clustering->initialize_pp();

    clock_t begin;
    clock_t end;

    cout << "[PERFORMING CLUSTERING]" << endl;

    // here we can adjust the max iterations of each clustering algorithm
    if (params.method == "Classic")
    {
        begin = clock();
        clustering->Lloyds(1000);
        end = clock();
    }
    else if (params.method == "LSH")
    {
        begin = clock();
        clustering->Reverse_Assignment_Cluestering(50);
        end = clock();
    }
    else if (params.method == "Hypercube")
    {
        begin = clock();
        clustering->Reverse_Assignment_Cluestering(500);
        end = clock();
    }

    double elapsed = double(end - begin) / CLOCKS_PER_SEC;

    ofstream output_file;
    output_file.open(params.out_file);

    cout << "[BUILDING OUTPUT]" << endl;

    if (params.complete)
    {
        for (int i = 0; i < params.clusters; i++)
        {
            output_file << "CLUSTER-" << i + 1 << " {centroid: [";
            for (int j = 0; j < dimension; j++)
            {
                output_file << clustering->centers[i].xij[j] << ",";
            }
            output_file << "]";
            for (int j = 0; j < clustering->clusters[i].size(); j++)
            {
                output_file << ", " << clustering->clusters[i][j].id;
            }
            output_file << "}" << endl;
        }
    }
    else
    {
        for (int i = 0; i < params.clusters; i++)
        {
            output_file << "CLUSTER-" << i + 1 << " {size: " << clustering->clusters[i].size() << ", centroid: [";
            for (int j = 0; j < dimension; j++)
            {
                output_file << clustering->centers[i].xij[j] << ",";
            }
            output_file << "]}" << endl;
        }
    }

    cout << "[CALCULATING SILHOUETTE]" << endl;
    if (clustering->centers.size() > 1)
    {
        output_file << "clustering_time: " << elapsed << endl;
        output_file << "Silhouette: [";
        for (int i = 0; i < params.clusters; i++)
        {
            // cout << i << endl;
            output_file << clustering->eval_specific_cluster(i) << ", ";
        }
        output_file << clustering->eval_clustering() << "]" << endl;
    }
    else
    {
        cout << "Silhouette can only be computed when there are 2 or more clusters." << endl;
    }

    output_file.close();

    delete clustering;
    return 0;
}
