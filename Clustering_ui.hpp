#ifndef CLUSTERING_UI_HPP
#define CLUSTERING_UI_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "utilities.hpp"

using namespace std;

class Cluster_params
{
public:
    string input_file = "./datasets/input_small_id"; // input file
    string conf_file = "./cluster.conf";             // configuration file
    string out_file = "./";                          // output file
    bool complete = false;                           // true = print contents of clusters
    string method = "Classic";                       // Classic, LSH or Hypercube
    int clusters;                                    // K of K-medians
    int L;                                           // number of vector hash tables (default=3)
    int k_LSH;                                       // number of vector hash functions (default=4)
    int M;                                           // max number of points to be checked, i.e. M of Hypercube (default=10)
    int k_HC;                                        // number of Hypercube dimensions, i.e. k of Hupercube (default=3)
    int probes;                                      // number of probes of Hypercube (default=2)
};

Cluster_params CMD_Interface(int argc, char *argv[])
{
    Cluster_params params;
    vector<string> args;
    for (int i = 1; i < argc; ++i)
    {
        args.push_back(string(argv[i]));
    }

    if ((args[0] == "-i") && (args[2] == "-c") && (args[4] == "-o") && ((args[6] == "-m") || (args[6] == "-complete" && args[7] == "-m")))
    {
        params.input_file = args[1];
        params.conf_file = args[3];
        params.out_file = args[5];
        if (args[6] == "-m")
        {
            if (args[7] == "Classic" || args[7] == "LSH" || args[7] == "Hypercube")
            {
                params.method = args[7];
            }
            else
            {
                cout << "Method entered is not an option" << endl;
            }
        }
        else
        {
            params.complete = true;

            if (args[8] == "Classic" || args[8] == "LSH" || args[8] == "Hypercube")
            {
                params.method = args[8];
            }
            else
            {
                cout << "Method entered is not an option" << endl;
            }
        }
    }
    else
    {
        cout << "Something went wrong with the given input." << endl
             << "Input should follow the following template:" << endl
             << "$./Clustering -i <input file> -c <configuration file> -o <output file> -complete <optional> -m <method: Classic OR LSH or Hypercube>" << endl;
        exit(-1);
    }
    return params;
}

int configuration(Cluster_params &params)
{
    string line;
    ifstream f;
    f.open(params.conf_file);
    if (f.is_open())
    {
        cout << "File " << params.conf_file << " successfully opened" << endl;
    }
    else
    {
        cout << "Error opening file " << params.conf_file << endl;
    }

    vector<int> config_values{0, 3, 4, 10, 3, 2}; // begin with default values
    int line_count = 0;
    while (!f.eof())
    {
        if (getline(f, line))
        {
            int extraction = extract_int_from_string(line);

            if (line_count == 0)
            {
                if (extraction == -1)
                {
                    cout << "Could not find configuration value in line " << line_count + 1 << " of configuration file." << endl
                         << "Please check that there is a space after the colon." << endl;
                    return -1;
                }
                config_values[line_count] = extraction;
            }
            else
            {
                if (extraction == -1)
                {
                    cout << "Could not find configuration value in line " << line_count + 1 << " of configuration file. Setting default value." << endl
                         << "(If you disagree please check that you are using a space after the colon)" << endl;
                }
                else
                {
                    config_values[line_count] = extraction;
                }
            }
        }
        line_count++;
    }
    params.clusters = config_values[0];
    params.L = config_values[1];
    params.k_LSH = config_values[2];
    params.M = config_values[3];
    params.k_HC = config_values[4];
    params.probes = config_values[5];

    return 0;
}

void print_Cluster_params(Cluster_params params)
{
    cout << "[Current parameters for Clustering]" << endl
         << "Input file: " << params.input_file << endl
         << "Configuration file: " << params.conf_file << endl
         << "Complete: " << params.complete << endl
         << "Method: " << params.method << endl
         << "Number of clusters: " << params.clusters << endl
         << "Number of hash tables: " << params.L << endl
         << "Number of hash functions: " << params.k_LSH << endl
         << "M of Hypercube: " << params.M << endl
         << "k of Hypercube: " << params.k_HC << endl
         << "probes: " << params.probes << endl
         << "Output file: " << params.out_file << endl;
}

#endif