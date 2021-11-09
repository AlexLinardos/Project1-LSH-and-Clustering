#include <iostream>
#include <vector>
#include "utilities.hpp"
#include "Clustering_ui.hpp"

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
        Cluster_params params;
        params = CMD_Interface(argc, argv);
        if (configuration(params) != 0)
        {
            return -1;
        }
        print_Cluster_params(params);
    }
    return 0;
}