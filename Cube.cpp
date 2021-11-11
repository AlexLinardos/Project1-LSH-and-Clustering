#include <iostream>
#include <vector>
#include <bitset>
#include "utilities.hpp"
#include "Cube_ui.hpp"
#include "hashing.hpp"
#include "Cube.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    Cube_params params;

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
    print_Cube_params(params);
    cout << "----------------------------------------" << endl;
    vector<Item> dataset;
    dataset = read_items(params.input_file);

    vector<Item> queries;
    queries = read_items(params.query_file);

    F f = F(params.k);
    Hypercube cube = Hypercube(params, dataset[0].xij.size(), 6, dataset, f.h_maps);

    cout << "----------------------------------------" << endl;
    cout << "[Now running kNN]" << endl;
    std::vector<std::pair<int, Item *>> knns = cube.kNN(queries[0]);
    for (int a = 0; a < params.N; a++)
    {
        cout << knns[a].first << ", " << knns[a].second->id << endl;
    }
    cout << "----------------------------------------" << endl;
    cout << "[Now running Range Search]" << endl;
    std::vector<std::pair<int, Item *>> rns = cube.RangeSearch(queries[0]);
    for (int a = 0; a < params.N; a++)
    {
        cout << rns[a].first << ", " << rns[a].second->id << endl;
    }
    return 0;
}