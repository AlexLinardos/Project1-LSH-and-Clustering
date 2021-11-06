#include <iostream>
#include <vector>
#include "utilities.hpp"
#include "LSH_ui.hpp"
#include "hashing.hpp"
#include "LSH.hpp"

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
    cout << "[HASH FUNCTION TESTING]" << endl;
    H test_H = H(4, item2.xij.size(), 4);
    vector<int> v;
    for (int i = 0; i < params.k; i++)
    {
        cout << test_H.produce_h(dataset[0]) << ", ";
    }
    // v= test_H.produce_k_h(dataset[0]);
    // for(int j=0; j<4;j++)
    // {
    //     cout << v[j] << ", ";
    // }
    // cout << endl;
    // int tableSize=dataset.size()/4;
    // G test_G = G(4, tableSize, 4, item2.xij.size());
    // for (int i = 0; i < 100; i++)
    // {
    //     //test_G.produce_g(dataset[i]);
    //     cout << test_G.produce_g(dataset[i]) << ", ";
    // }

    // // Initialize L hash tables and L g-HashFunctions
    // std::vector<Item*>** hashTables= new std::vector<Item *>*[params.L];
    // G** g = new G*[params.L];

    // for (int i = 0; i < params.L; i++) // for every hashTable
    // {
    //     hashTables[i]=new std::vector<Item*>[tableSize];
    //     g[i] = new G(params.k, tableSize, 4, item2.xij.size());
    //     cout << g[i]->produce_g(dataset[0]) << endl;
    // }
    cout << endl;
    cout << "Run LSH..." << endl;
    
    return 0;
}