#ifndef UTILITIES_HPP
#define UTILITIES_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <cmath>
#include <unordered_map>

using namespace std;

// Κάθε γραμμή του dataset/query είναι ένα Item με id και ένα vector από ακεραίους
class Item
{
public:
    string id;
    vector<double> xij;

    bool null = false; // Flag if is NULL
    Item()
    {
        id = "-1";
        null = true;
    }

    Item(string id, vector<double> xij) : id(id), xij(xij) { null = false; }
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
                    int xij_int = stod(out[i]);
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
double EuclideanDistance(Item *p, Item *q, const uint16_t &d)
{
    double sum = 0;
    for (int i = 0.0; i < d; i++)
    {
        sum += (p->xij[i] - q->xij[i]) * (p->xij[i] - q->xij[i]);
    }

    return sqrt(sum);
}

bool comparePairs(std::pair<int, Item *> x, std::pair<int, Item *> y)
{
    return (x.first < y.first);
}

int extract_int_from_string(string str)
{
    stringstream ss;
    ss << str;
    string word;
    int temp_int;

    while (!ss.eof())
    {
        ss >> word;
        if (stringstream(word) >> temp_int)
        {
            ss.str(string());
            return temp_int;
        }
    }
    ss.str(string());
    return -1;
}

template <typename K, typename V>
void print_map(std::unordered_map<K, V> const &m)
{
    for (auto const &pair : m)
    {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

int HammingDistance(unsigned int i1, unsigned int i2)
{
    int x = i1 ^ i2;
    int setBits = 0;

    while (x > 0)
    {
        setBits += x & 1;
        x >>= 1;
    }
    return setBits;
}

// performs the operation of vector addition
vector<double> vector_mean(vector<double> &v1, vector<double> v2, int dimensions, int T)
{
    vector<double> result(dimensions, 0);
    for (int i = 0; i < dimensions; ++i)
    {
        result[i] = v1[i] + (v2[i] / T);
    }
    return result;
}

#endif