#include <iostream>
#include <vector>
#include "utilities.hpp"
#include "Clustering_ui.hpp"
#include "Clustering.hpp"
#include <map>
#include <random>
#include <chrono>
using namespace std;

int main(int argc, char *argv[])
{
    Cluster_params params;
    if (argc == 1)
    {
        // cout << "Run program from command line following the format below: " << endl;
        // cout << "$./cluster -i <input file> -c <configuration file> -o <output file> -complete <optional> -m <method: Classic OR LSH or Hypercube>" << endl;
        // return -1;
        params.input_file = "./datasets/input_small_id";
        params.conf_file = "./cluster.conf";
        params.out_file = "./out.txt";
        params.complete = false;
        params.method = "LSH";
        params.clusters = 5;
        params.L = 3;
        params.k_LSH = 4;
        params.M = 10;
        params.k_HC = 3;
        params.probes = 2;
        print_Cluster_params(params);
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
    Clustering *cluster = new Clustering(params, dataset);
    int dimension = dataset[0].xij.size();

    cluster->initialize_pp();

    clock_t begin;
    clock_t end;

    if(params.method=="Classic")
    {   
        begin = clock();
        cluster->Lloyds(1000);
        end = clock();
    }
    else
    {   
        begin = clock();
        cluster->Reverse_Assignment_Cluestering();
        end = clock();
    }

    double elapsed = double(end - begin) / CLOCKS_PER_SEC;

    ofstream output_file;
    output_file.open("CLUSTERING_output.txt");

    if(params.complete)
    {
        for (int i = 0; i < params.clusters; i++)
        {
            output_file << "CLUSTER-" << i+1 << " {centroid: [";
            for(int j=0; j< dimension; j++)
            {
                output_file << cluster->centers[i].xij[j] << ",";
            }
            output_file << "]";
            for(int j=0; j< cluster->clusters[i].size(); j++)
            {
                output_file << ", " << cluster->clusters[i][j].id;
            }
            output_file << "}" << endl;
        }
    }
    else
    {
        for (int i = 0; i < params.clusters; i++)
        {
            output_file << "CLUSTER-" << i+1 << " {size: " << cluster->clusters[i].size() << ", centroid: [";
            for(int j=0; j< dimension; j++)
            {
                output_file << cluster->centers[i].xij[j] << ",";
            }
            output_file << "]}" << endl;
        }
    }
    if (cluster->centers.size() > 1)
    {
        output_file << "clustering_time: " << elapsed << endl;
        output_file << "Silhouette: [";
        for (int i = 0; i < params.clusters; i++)
        {
            cout << i << endl;
            output_file << cluster->eval_specific_cluster(i) << ", ";
        }
        output_file << cluster->eval_clustering() << "]" << endl;    
    }
    else
    {
        cout << "Silhouette can only be computed when there are 2 or more clusters." << endl;
    }

    output_file.close();

    delete cluster;
    return 0;
}
