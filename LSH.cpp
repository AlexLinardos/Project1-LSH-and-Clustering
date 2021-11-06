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
    // H test_H = H(4, item2.xij.size(), 4);
    // vector<int> v;
    // for (int i = 0; i < 10; i++)
    // {
    //     cout << test_H.produce_h(dataset[0]) << ", ";
    // }
    // v= test_H.produce_k_h(dataset[0]);
    // for(int j=0; j<4;j++)
    // {
    //     cout << v[j] << ", ";
    // }
    // cout << endl;
    G test_G = G(4, dataset.size() / 4, 4, item2.xij.size());
    for (int i = 0; i < 100; i++)
    {
        // test_G.produce_g(dataset[i]);
        cout << test_G.produce_g(dataset[i]) << ", ";
    }
    cout << endl;
    cout << "Run LSH..." << endl;
    return 0;
}