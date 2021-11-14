#ifndef LSH_UI_HPP
#define LSH_UI_HPP
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// class to hold parameters of LSH algorithm
class LSH_params
{
public:
    string input_file = "./datasets/input_small_id";
    string query_file = "./datasets/query_small_id";
    string out_file = "./";
    int k = 4;                 // number of hi functions
    int L = 5;                 // number of hash tables
    int N = 1;                 // number of nearest neighbors to be found
    double R = 10000;          // search range
    string source = "default"; // shows if parameters are default or if they chenged (values: "default", "custom")
};

// interface used when no command line arguments are given at execution
LSH_params Guided_Interface()
{
    LSH_params params;
    params.input_file = "./datasets/input_small_id";
    params.query_file = "./datasets/query_small_id";
    params.N = 5;
    params.R = 500;
    // params.L = 7;
    // params.k = 5;

    // string answer;
    // cout << "Enter dataset file path: ";
    // cin >> answer;
    // params.input_file = answer;
    // cout << "Enter query file path: ";
    // cin >> answer;
    // params.query_file = answer;

    // cout << "Without command line parameters the program will run with default values:" << endl
    //      << "k = 4 (hi functions)" << endl
    //      << "L = 5 (hash tables)" << endl
    //      << "N = 1 (number of nearest neighbors)" << endl
    //      << "R = 10000 (search range)" << endl
    //      << endl
    //      << "Would you like to change these values; (y/n) ";
    // cin >> answer;

    // if (answer == "y" || answer == "yes" || answer == "Y" || answer == "YES" || answer == "Yes")
    // {
    //     int answer;
    //     cout << "Enter new values" << endl;
    //     cout << "k = ";
    //     cin >> answer;
    //     params.k = answer;
    //     cout << "L = ";
    //     cin >> answer;
    //     params.L = answer;
    //     cout << "N = ";
    //     cin >> answer;
    //     params.N = answer;
    //     cout << "R = ";
    //     double answer_R;
    //     cin >> answer_R;
    //     params.R = answer_R;
    //     params.source = "custom";
    // }
    return params;
}

// interface used when program is executed with command line arguments
LSH_params CMD_Interface(int argc, char *argv[])
{
    LSH_params params;
    vector<string> args;
    for (int i = 1; i < argc; ++i)
    {
        args.push_back(string(argv[i]));
    }

    if ((args[0] == "-i") && (args[2] == "-q") && (args[4] == "-k") && (args[6] == "-L") && (args[8] == "-o") && (args[10] == "-N") && (args[12] == "-R"))
    {
        params.input_file = args[1];
        params.query_file = args[3];
        params.k = stoi(args[5]);
        params.L = stoi(args[7]);
        params.out_file = args[9];
        params.N = stoi(args[11]);
        params.R = stod(args[13]);
        params.source = "custom";
    }
    else
    {
        cout << "Something went wrong with the given input." << endl
             << "Input should follow the following template:" << endl
             << "$./lsh –i <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius> " << endl;
    }
    return params;
}

#endif

void print_LSH_params(const LSH_params &params)
{
    cout << "[Current parameters for LSH]" << endl
         << "Input file: " << params.input_file << endl
         << "Query file: " << params.query_file << endl
         << "k: " << params.k << endl
         << "L: " << params.L << endl
         << "N: " << params.N << endl
         << "R: " << params.R << endl
         << "Output file: " << params.out_file << endl;
}