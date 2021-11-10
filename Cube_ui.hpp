#ifndef CUBE_UI_HPP
#define CUBE_UI_HPP
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Κλάση που κρατάει τις παραμέτρους του υπερκύβου
class Cube_params
{
public:
    string input_file = "./datasets/input_small_id";
    string query_file = "./datasets/query_small_id";
    string out_file = "./";
    int k = 14;                // Διάσταση στην οποία προβάλλονται τα σημεία
    int M = 1000;              // Μέγιστο πλήθος υποψήφιων σημέιων που θα ελεγχθούν
    int probes = 20;           // Μέγιστο επιτρεπόμενο πλήθος κορυφών που θα ελεγχθούν
    int N = 5;                 // Αριθμός πλησιέστερων γειτόνων
    double R = 1000;           // Ακτίνα αναζήτησης
    string source = "default"; // Δείχνει αν οι παράμετροι είναι οι default ή αν έχουν αλλάξει (τιμές: "default", "custom")
};

Cube_params Guided_Interface()
{
    Cube_params params;
    params.input_file = "./datasets/input_small_id";
    params.query_file = "./datasets/query_small_id";

    /*
    string answer;
    cout << "Enter dataset file path: ";
    cin >> answer;
    params.input_file = answer;
    cout << "Enter query file path: ";
    cin >> answer;
    params.query_file = answer;

    cout << "Without command line parameters the program will run with default values:" << endl
         << "k = 14 (dimensions)" << endl
         << "M = 10 (maximum number of points to be checked)" << endl
         << "probes = 2 (maximum number of edges to be checked)" << endl
         << "N = 1 (number of nearest neighbors)" << endl
         << "R = 10000 (search range)" << endl
         << endl
         << "Would you like to change these values; (y/n) ";
    cin >> answer;

    if (answer == "y" || answer == "yes" || answer == "Y" || answer == "YES" || answer == "Yes")
    {
        int answer;
        cout << "Enter new values" << endl;
        cout << "k = ";
        cin >> answer;
        params.k = answer;
        cout << "M = ";
        cin >> answer;
        params.M = answer;
        cout << "probes = ";
        cin >> answer;
        params.probes = answer;
        cout << "N = ";
        cin >> answer;
        params.N = answer;
        cout << "R = ";
        double answer_R;
        cin >> answer_R;
        params.R = answer_R;
        params.source = "custom";
    }
    */
    return params;
}

Cube_params CMD_Interface(int argc, char *argv[])
{
    Cube_params params;
    vector<string> args;
    for (int i = 1; i < argc; ++i)
    {
        args.push_back(string(argv[i]));
    }

    if ((args[0] == "-i") && (args[2] == "-q") && (args[4] == "-k") && (args[6] == "-M") && (args[8] == "-probes") && (args[10] == "-o") && (args[12] == "-N") && (args[14] == "-R"))
    {
        params.input_file = args[1];
        params.query_file = args[3];
        params.k = stoi(args[5]);
        params.M = stoi(args[7]);
        params.probes = stoi(args[9]);
        params.out_file = args[11];
        params.N = stoi(args[13]);
        params.R = stod(args[15]);
        params.source = "custom";
    }
    else
    {
        cout << "Something went wrong with the given input." << endl
             << "Input should follow the following template:" << endl
             << "$./cube –i <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file> -Ν <number of nearest> -R <radius> " << endl;
    }
    return params;
}

void print_Cube_params(Cube_params params)
{
    cout << "[Current parameters for Hypercube]" << endl
         << "Input file: " << params.input_file << endl
         << "Query file: " << params.query_file << endl
         << "k: " << params.k << endl
         << "M: " << params.M << endl
         << "probes: " << params.probes << endl
         << "N: " << params.N << endl
         << "R: " << params.R << endl
         << "Output file: " << params.out_file << endl;
}

#endif