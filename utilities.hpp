#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <typeinfo>

using namespace std;

// Κάθε γραμμή του dataset είναι ένα Item με id και ένα vector από ακεραίους
class Item
{
public:
    string id;
    vector<int> xij;
};

// Συνάρτηση διαμέρισης γραμμής κειμένου στα ξεχωριστά στοιχεία της
// βάσει ενός delimeter (πχ. space, tab, κλπ)
void tokenize(string const &str, const char delim, vector<string> &out)
{
    stringstream ss(str);

    string s;
    while (getline(ss, s, delim))
    {
        out.push_back(s);
    }
}

// Συνάρτηση διαβάσματος dataset από αρχείο filename με delimiter delim
vector<Item> read_dataset(string filename, const char delim)
{
    vector<Item> data;
    string line;

    ifstream f;
    f.open(filename);
    if (f.is_open())
    {
        cout << "File " << filename << " successfully opened" << endl;
    }
    else
    {
        cout << "Error opening file " << filename << endl;
    }

    int count = 1;
    while (!f.eof())
    {
        if (getline(f, line))
        {
            vector<string> out;
            tokenize(line, delim, out);

            Item item;
            item.id = out[0];
            // Μετατροπή των values του διανύσματος από string σε int
            for (size_t i = 1; i < out.size(); ++i)
            {
                int xij_int = stoi(out[i]);
                item.xij.push_back(xij_int);
            }

            data.push_back(item);
            count += 1;
        }
        /*
        else
        {
            cout << "Failed to read file at line " << count << endl;
        }
        */
    }
    f.close();
    return data;
}

#endif