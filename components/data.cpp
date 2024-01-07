#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <limits>

using namespace std;

struct pair_hash {
    template <typename T1, typename T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
    }
};

class Dataset
{
public:
    string path;
    vector<pair<string, string>> pairs;
    set<string> vocab;
    set<string> POS;
    unordered_map<string, int> tag_freq;
    unordered_map <pair<string, string>, int, pair_hash> transition_freq;
    unordered_map <pair<string, string>, int, pair_hash> emission_freq;
    unordered_map <string, int> prior_freq;
    unordered_map <pair<string, string>, double, pair_hash> transition_probs;
    unordered_map <pair<string, string>, double, pair_hash> emission_probs;
    unordered_map <string, double> prior_probs;

    Dataset()
    {
    }
    Dataset(string s)
    {
        path = s;
    }
    void load_dataset();
    pair<string, string> process_line(string line);
    void create_vocabulary();
    void create_dictionary();
    void calculate_probs();
};

void Dataset::load_dataset()
{
    ifstream dataset;
    dataset.open(path);
    string s;

    // adding start tag to represent start of text
    pairs.push_back(make_pair("--s---", "--s--"));

    if (dataset.is_open())
    {
        cout << "Processing Dataset..." << endl;
        while (getline(dataset, s))
        {
            pair<string, string> p = process_line(s);
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
}

pair<string, string> Dataset::process_line(string line)
{
    if (line.length() == 0)
    {
        return make_pair("--s---", "--s--");
    }
    string word1, word2;
    int i = 0;
    while (line[i] != '\t')
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

void Dataset::create_vocabulary()
{
    for (auto p : pairs)
    {
        vocab.insert(p.first);
        POS.insert(p.second);
    }
    
    // adding unknown tag to represent unknown words
    POS.insert("UNK");
    vocab.insert("UNK");

    cout << "There are " << vocab.size() << " Unique words in dataset" << endl;
    cout << "There are " << POS.size() << " Unique POS tags in dataset" << endl;
}

void Dataset::create_dictionary()
{
    for (int i = 0; i < pairs.size()-1; i++)
    {
        transition_freq[make_pair(pairs[i].first, pairs[i+1].first)] += 1;
        emission_freq[pairs[i]] += 1;
        tag_freq[pairs[i].second] += 1;

        if (pairs[i].first == "--s--")
        {
            prior_freq[pairs[i+1].first] += 1;
        }
    }
}

void Dataset::calculate_probs()
{
    double epsilon = numeric_limits<double>::epsilon();

    for (auto p: transition_freq)
    {
        transition_probs[make_pair(p.first.first, p.first.second)] = (p.second + epsilon)/ (tag_freq[p.first.first] + POS.size()*epsilon);
    }

    for (auto p: emission_freq)
    {
        emission_probs[make_pair(p.first.first, p.first.second)] = (p.second + epsilon)/ (tag_freq[p.first.first] + vocab.size()*epsilon);
    }

    for (auto p: prior_freq)
    {
        prior_probs[p.first] = (p.second + epsilon)/ (pairs.size() + POS.size()*epsilon);
    }
}