#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <utility>
#include <algorithm>

using namespace std;

// function declaration
vector <pair <string, string>> load_dataset(string path);
pair <string, string> process_line(string line);

// function definition
vector <pair <string, string>> load_dataset(string path)
{
    ifstream dataset;
    dataset.open(path);
    string s;

    // create a vector of pairs to store mapping of words and tags
    vector <pair <string, string>> pairs;

    // adding start tag to represent start of text
    pairs.push_back(make_pair("--s---", "--s--"));

    if (dataset.is_open())
    {
        cout << "Processing Dataset..." << endl;
        while (getline(dataset, s))
        {
            pair <string, string> p = process_line(s);
            pairs.push_back(p);
        }
        cout << "Processing Dataset Complete" << endl;
    }
    else
    {
        cout << "Unable to open file";
        exit(1);
    }

    dataset.close();

    return pairs;
}

pair <string, string> process_line(string line)
{
    if (line.length() == 0)
    {
        return make_pair("--s---", "--s--");
    }
    string word1, word2;
    int i = 0;
    while (line[i]!= '\t')
    {
        word1 += line[i];
        i++;
    }
    i++;
    while (i != line.length())
    {
        word2 += line[i];
        i++;
    }
    return make_pair(word1, word2);
}

int main()
{  
    string path = "data/sample.pos";

    vector <pair <string, string>> pairs = load_dataset(path);

    // printing the pairs for debugging
    for (auto p : pairs)
    {
        cout << p.first << " " << p.second << endl;
    }

    return 0;
}