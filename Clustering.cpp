#include <iostream>
#include <vector>
#include "utilities.hpp"
#include "Clustering_ui.hpp"
#include "Clustering2.hpp"
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
        params.method = "Classic";
        params.clusters = 3;
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
    Alekos::Clustering cluster = Alekos::Clustering(params, dataset);

    cluster.initialize_pp();
    cluster.Lloyds(1000);

    return 0;
}