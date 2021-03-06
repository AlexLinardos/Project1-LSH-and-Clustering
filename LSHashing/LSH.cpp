#include <iostream>
#include <vector>
#include "../misc/utilities.hpp"
#include "LSH.hpp"
#include <chrono>
#include <random>

using namespace std;

int main(int argc, char *argv[])
{
    lshui::LSH_params params;

    if (argc == 1) // Αν δεν έχουν δωθεί παράμετροι από το terminal τρέξε το interface με prompts
    {
        params = lshui::Guided_Interface();
    }
    else // Αλλιώς διάβασε τις παραμέτρους από το terminal
    {
        params = lshui::CMD_Interface(argc, argv);
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

    LSH * lsh = new LSH(params, dataset, 3, 8);

    std::vector<std::pair<double, Item *>> knns;
    std::vector<std::pair<double, Item *>> true_knns;
    std::vector<std::pair<double, Item *>> r;

    std::chrono::steady_clock::time_point lsh_begin;
    std::chrono::steady_clock::time_point lsh_end;
    std::chrono::steady_clock::time_point true_begin;
    std::chrono::steady_clock::time_point true_end;

    double error = 0.0;
    double avg_error = 0.0;

    ofstream output_file;
    output_file.open(params.out_file);
    double lsh_elapsed = 0.0;
    double brute_elapsed = 0.0;
    clock_t begin;
    clock_t end;

    for (int i = 0; i < queries.size(); i++)
    {
        output_file << "Query: " << queries[i].id << endl;

        // cout << "[k-ANN]" << endl;
        lsh_begin = std::chrono::steady_clock::now();
        begin = clock();
        knns = lsh->kNN(&queries[i], dataset.size() / 5);
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
        int neighboors_returned = 0;

        for (int j = 0; j < params.N; j++)
        {
            if (knns[j].second->null==true)
            {
                output_file << "Nearest neighbor-" << j + 1 << ": "
                            << "NOT FOUND" << endl;
                continue;
            }
            output_file << "Nearest neighbor-" << j + 1 << ": " << knns[j].second->id << endl;
            output_file << "distanceLSH: " << knns[j].first << endl;
            output_file << "distanceTrue: " << true_knns[j].first << endl;
            error += (knns[j].first / true_knns[j].first); // sum distLSH/distTrue of the N nearest neigbors of a Query
            neighboors_returned++;
        }
        output_file << "tLSH: " << (std::chrono::duration_cast<std::chrono::microseconds>(lsh_end - lsh_begin).count()) / 1000000.0 << std::endl;
        output_file << "tTrue: " << (std::chrono::duration_cast<std::chrono::microseconds>(true_end - true_begin).count()) / 1000000.0 << std::endl;

        output_file << "R-near neighbors:" << endl;
        r = lsh->RangeSearch(&queries[i], params.R, dataset.size() / 5);
        for (int a = 0; a < r.size(); a++)
        {
            output_file << r[a].second->id << ", " << r[a].first << endl;
        }
        
        if (error != 0)
        {
            avg_error += error / (double)neighboors_returned; // sum avg(distLSH/distTrue) of the N nearest neigbors of a Query for all the Queries
            error = 0;
        }
        neighboors_returned =0;
    }

    cout << "[EVALUATION]" << endl;

    lsh_elapsed = lsh_elapsed / CLOCKS_PER_SEC;
    brute_elapsed = brute_elapsed / CLOCKS_PER_SEC;
    cout << "tlSH/tTrue: " << (lsh_elapsed * 1000000.0) / (brute_elapsed * 1000000.0) << endl;

    avg_error = avg_error / (double)queries.size(); // calculate avg distLSH/distTrue across all Queries 

    cout << "distLSH/distTrue (avg): " << avg_error << endl;

    output_file.close();

    delete lsh;
    return 0;
}
