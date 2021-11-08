#ifndef UTILITIES_HPP
#define UTILITIES_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <cmath>

using namespace std;

// Κάθε γραμμή του dataset/query είναι ένα Item με id και ένα vector από ακεραίους
class Item
{
public:
    string id;
    vector<int> xij;
    
    bool null= false; // Flag if is NULL
    Item () 
    {
        id = "-1";
        null = true;
    }

    Item (string id, vector<int> xij) : id(id), xij(xij) {null = false;}
};

// Συνάρτηση διαμέρισης γραμμής κειμένου στα ξεχωριστά στοιχεία της
vector<string> tokenize(string str)
{
    vector<string> tokens;
    string tmp;
    stringstream str_strm(str);
    while (str_strm >> tmp)
    {
        tokens.push_back(tmp);
    }
    return tokens;
}

// Συνάρτηση διαβάσματος dataset/query από αρχείο filename με delimiter delim
vector<Item> read_items(string filename)
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
    int catch_count = 0;
    int line_count = 1;
    while (!f.eof())
    {
        if (getline(f, line))
        {
            line_count++;
            vector<string> out;
            out = tokenize(line);
            Item item;
            item.id = out[0];
            item.null = false;

            // Μετατροπή των values του διανύσματος από string σε int
            for (size_t i = 1; i < out.size(); ++i)
            {
                try
                {
                    int xij_int = stoi(out[i]);
                    item.xij.push_back(xij_int);
                }
                catch (...)
                {
                    cout << "[Line:" << line_count << "] Here is the catch: " << out[i] << "." << endl;
                    cout << "[Line:" << line_count << "] Type: " << typeid(out[i]).name() << endl;
                    catch_count++;
                }
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
    // cout << "Catches: " << catch_count << endl;

    f.close();
    return data;
}

/* function that computes the Euclidean Distance between 2 points of dimension d */
double EuclideanDistance(Item* p, Item* q, const uint16_t& d)
{
    double sum = 0;
    for (int i = 0.0; i < d; i++)
    {
    sum += (p->xij[i] - q->xij[i]) * (p->xij[i] - q->xij[i]);
    }

    return sqrt(sum);
}

bool comparePairs (std::pair<int, Item*> x, std::pair<int, Item*> y) {
    return (x.first < y.first);
}

#endif