#include <iostream>
#include <vector>
#include "LSH_ui.h"
#include "utilities.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "[HYPOTHETIC RUN]" << endl;
    cout << "Check for command line arguments..." << endl;
    cout << "Read input file (dataset)..." << endl;
    vector<Item> dataset;
    dataset = read_dataset("./datasets/input_small_id");
    Item item = dataset.at(9999);
    cout << "Vector ID: " << item.id << endl;
    cout << "Vector Values: ";
    for (size_t i = 0; i < item.xij.size(); ++i)
    {
        cout << item.xij[i] << " ";
    }
    cout << endl;
    cout << "Read query file..." << endl;
    cout << "Run LSH..." << endl;
    return 0;
}