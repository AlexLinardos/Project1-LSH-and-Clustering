#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <typeinfo>
// hello by tony

using namespace std;

// Κάθε γραμμή του dataset είναι ένα Item με id και ένα vector από floats
class Item
{
public:
    string id;
    vector<float> xij;
};

void tokenize(string const &str, const char delim, vector<string> &out)
{
    stringstream ss(str);

    string s;
    while (getline(ss, s, delim))
    {
        out.push_back(s);
    }
}

vector<Item> read_dataset(string filename, const char delim)
{
    vector<Item> data;
    string line;

    ifstream f;
    f.open(filename);

    while (!f.eof())
    {
        if (getline(f, line))
        {
            vector<string> out;
            tokenize(line, delim, out);

            Item item;
            item.id = out[0];
            // Μετατροπή των values από string σε float
            float xi1_float = stof(out[1]);
            item.xij.push_back(xi1_float);
            float xi2_float = stof(out[2]);
            item.xij.push_back(xi2_float);

            data.push_back(item);
        }
        else
        {
            cout << "Failed to read dataset file..." << endl;
        }
    }
    f.close();
    return data;
}

int main(void)
{
    vector<Item> dataset;
    dataset = read_dataset("./test_dataset.txt", '\t');

    for (int i = 0; i < dataset.size(); i++)
    {
        Item item = dataset.at(i);
        cout << "ID: " << item.id << ", X" << i << "1: " << item.xij[0] << ", X" << i << "2: " << item.xij[1] << endl;
    }
}
