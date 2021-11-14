#include <iostream>
#include <vector>
#include "utilities.hpp"
#include "hashing.hpp"
#include "LSH.hpp"
#include <chrono>
#include <random>

using namespace std;

int main(int argc, char *argv[])
{
    LSH_params params;

    if (argc == 1) // Αν δεν έχουν δωθεί παράμετροι από το terminal τρέξε το interface με prompts
    {
        params = Guided_Interface();
    }
    else // Αλλιώς διάβασε τις παραμέτρους από το terminal
    {
        params = CMD_Interface(argc, argv);
        if (params.source == "default") // Αν κάποιο option έχει γραφτεί λάθος τότε οι παράμετροι δεν θα αλλάξουν (θα παραμείνουν οι default)
        {
            return -1; // Κλείσε το πρόγραμμα
        }
    }
    print_LSH_params(params);

    vector<Item> dataset;
    read_items(dataset, params.input_file);
    vector<Item> queries;
    read_items(queries, params.query_file);

    cout << "[RUN LSH]" << endl;

    LSH lsh = LSH(params, dataset, 3, 8);

    std::vector<std::pair<double, Item *>> knns;
    std::vector<std::pair<double, Item *>> true_knns;
    std::vector<std::pair<double, Item *>> r;

    std::chrono::steady_clock::time_point lsh_begin;
    std::chrono::steady_clock::time_point lsh_end;
    std::chrono::steady_clock::time_point true_begin;
    std::chrono::steady_clock::time_point true_end;

    double error = 0;
    double meso_error = 0;

    ofstream output_file;
    output_file.open("LSH_output.txt");
    double lsh_elapsed = 0;
    double brute_elapsed = 0;
    clock_t begin;
    clock_t end;

    for (int i = 0; i < queries.size(); i++)
    {
        output_file << "Query: " << queries[i].id << endl;

        // cout << "[k-ANN]" << endl;
        lsh_begin = std::chrono::steady_clock::now();
        begin = clock();
        knns = lsh.kNN(&queries[i], dataset.size() / 5);
        end = clock();
        lsh_end = std::chrono::steady_clock::now();
        lsh_elapsed += double(end - begin);

        // cout << "[Brute Force]" << endl;
        true_begin = std::chrono::steady_clock::now();
        begin = clock();
        true_knns = brute_force_search(dataset, &queries[i], params.N);
        end = clock();
        true_end = std::chrono::steady_clock::now();
        brute_elapsed += double(end - begin);

        for (int j = 0; j < params.N; j++)
        {
            output_file << "Nearest neighbor-" << j + 1 << ": " << knns[j].second->id << endl;
            output_file << "distanceLSH: " << knns[j].first << endl;
            output_file << "distanceTrue: " << true_knns[j].first << endl;
            error += (knns[j].first / true_knns[j].first);
        }
        output_file << "tLSH: " << (std::chrono::duration_cast<std::chrono::microseconds>(lsh_end - lsh_begin).count()) / 1000000.0 << std::endl;
        output_file << "tTrue: " << (std::chrono::duration_cast<std::chrono::microseconds>(true_end - true_begin).count()) / 1000000.0 << std::endl;

        // output_file << "R-near neighbors:" << endl;
        // r = lsh.RangeSearch(&queries[i], params.R, 0);
        // for (int a = 0; a < r.size(); a++)
        // {
        //     output_file << r[a].second->id << ", " << r[a].first << endl;
        // }
        meso_error += error / (double)params.N;
        error = 0;
    }

    cout << "[EVALUATION]" << endl;

    lsh_elapsed = lsh_elapsed / CLOCKS_PER_SEC;
    brute_elapsed = brute_elapsed / CLOCKS_PER_SEC;
    cout << "tlSH/tTrue: " << (lsh_elapsed * (double)1000000) / (brute_elapsed * (double)1000000) << endl;

    meso_error = meso_error / (double)queries.size();

    cout << "distLSH/distTrue (avg): " << meso_error << endl;

    output_file.close();
    return 0;
}