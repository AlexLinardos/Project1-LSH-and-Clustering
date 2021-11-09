#include <iostream>
#include <vector>
#include "utilities.hpp"
#include "LSH_ui.hpp"
#include "hashing.hpp"
#include "LSH.hpp"
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
    LSH_params params ;

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
    cout << "----------------------------------------" << endl;
    vector<Item> dataset;
    dataset = read_items(params.input_file);
    Item item = dataset.at(9999);
    cout << "[DATASET VECTOR TEST PRINT]" << endl;
    cout << "Vector ID: " << item.id << endl;
    cout << "Vector Values: ";
    for (size_t i = 0; i < item.xij.size(); ++i)
    {
        cout << item.xij[i] << " ";
    }
    cout << endl;
    cout << "----------------------------------------" << endl;
    vector<Item> queries;
    queries = read_items(params.query_file);
    Item item2 = queries.at(99);
    cout << "[QUERY VECTOR TEST PRINT]" << endl;
    cout << "Vector ID: " << item2.id << endl;
    cout << "Vector Values: ";
    for (size_t i = 0; i < item2.xij.size(); ++i)
    {
        cout << item2.xij[i] << " ";
    }
    cout << endl;
    cout << "----------------------------------------" << endl;
    //cout << tableSize << endl;
    // cout << "[HASH FUNCTION TESTING]" << endl;
    // H test_H = H(4, item2.xij.size(), 4);
    // vector<int> v;
    // for (int i = 0; i < params.k; i++)
    // {
    //     cout << test_H.produce_h(dataset[0]) << ", ";
    // }
    // v= test_H.produce_k_h(dataset[0]);
    // for(int j=0; j<4;j++)
    // {
    //     cout << v[j] << ", ";
    // }
    // cout << endl;
    // G test_G = G(4, tableSize, 4, item2.xij.size());
    // for (int i = 0; i < 100; i++)
    // {
    //     //test_G.produce_g(dataset[i]);
    //     cout << test_G.produce_g(dataset[i]) << ", ";
    // }
    // cout << "----------------------------------------" << endl;
    cout << "Run LSH..." << endl;

    LSH lsh = LSH(params, 4, 4);

    ofstream output_file;
    output_file.open ("LSH_output.txt");

    std::vector<std::pair<int, Item*>> knns;
    std::vector<std::pair<int, Item*>> true_knns;
    std::vector<std::pair<int, Item*>> r;

    std::chrono::steady_clock::time_point lsh_begin;
    std::chrono::steady_clock::time_point lsh_end;
    std::chrono::steady_clock::time_point true_begin;
    std::chrono::steady_clock::time_point true_end;

    for (int i =0; i<queries.size(); i++)
    {
        output_file << "Query: " << queries[i].id << endl;
        
        lsh_begin = std::chrono::steady_clock::now();
        knns = lsh.kNN(&queries[0], params.N, 0);
        lsh_end = std::chrono::steady_clock::now();
        
        true_begin = std::chrono::steady_clock::now();
        true_knns = lsh.brute_force_search(&queries[i], params.N);
        true_end = std::chrono::steady_clock::now();

        for (int j = 0; j < params.N; j++)
        {
            output_file << "Nearest neighbor-" << j+1 << ": " << knns[j].second->id << endl;
            output_file << "distanceLSH: " << knns[j].first << endl;
            output_file << "distanceTrue: " << true_knns[j].first << endl;
        }
        output_file << "tLSH: " <<  (std::chrono::duration_cast<std::chrono::microseconds>(lsh_end - lsh_begin).count()) /1000000.0  <<std::endl;
        output_file << "tTrue: " <<  (std::chrono::duration_cast<std::chrono::microseconds>(true_end - true_begin).count()) /1000000.0  <<std::endl;

        output_file << "R-near neighbors:" << endl;
        r = lsh.RangeSearch(&queries[0], params.R, 0);
        for (int a = 0; a < r.size(); a++)
        {
            output_file << r[a].second->id << ", " << r[a].first << endl;
        }
    }
    
    output_file.close();
    return 0;
}