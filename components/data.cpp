#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <limits>
#include <math.h>

using namespace std;

struct pair_hash 
{
    template <typename T1, typename T2>
    size_t operator()(const std::pair<T1, T2>& p) const 
    {
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
    unordered_map <pair<string, string>, int, pair_hash> prior_freq;
    unordered_map <string, int> prior_tag_freq;
    unordered_map <pair<string, string>, double, pair_hash> transition_probs;
    unordered_map <pair<string, string>, double, pair_hash> emission_probs;
    unordered_map <pair<string, string>, double, pair_hash> prior_probs;
    unordered_map <string, int> tag_to_idx;
    unordered_map <int, string> idx_to_tag;
    vector<vector<double>> dp;
    vector<vector<int>> tags;
    vector <string> answer;

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
    void count_frequencies();
    void calculate_probs();
};

void Dataset::load_dataset()
{
    ifstream dataset;
    dataset.open(path);
    string s;

    // adding start tag to represent start of text
    pairs.push_back(make_pair("--s--", "--s--"));

    if (dataset.is_open())
    {
        cout << "Processing Dataset..." << endl;
        while (getline(dataset, s))
        {
            // convert vocabulary to lowercase
            for (char &c : s) 
            {
                c = tolower(c);
            }
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
        // adding start tag to represent start of text
        return make_pair("--s--", "--s--");
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
        if (p.first == "--s--")
        {
            // do nothing for start tag
            continue;
        }
        vocab.insert(p.first);
        POS.insert(p.second);
    }
    
    // adding unknown tag to vocab to represent unknown words
    vocab.insert("UNK");

    cout << "There are " << vocab.size() << " Unique words in dataset" << endl;
    cout << "There are " << POS.size() << " Unique POS tags in dataset" << endl;
}

void Dataset::count_frequencies()
{
    bool isStart = false;
    for (int i = 0; i < pairs.size()-1; i++)
    {
        if (pairs[i].first == "--s--")
        {
            isStart = true;
            continue;
        }

        if (i < pairs.size()-2 && pairs[i+1].first!= "--s--")
        {
            transition_freq[make_pair(pairs[i].second, pairs[i+1].second)] += 1;
        }

        emission_freq[make_pair(pairs[i].second, pairs[i].first)] += 1;
        tag_freq[pairs[i].second] += 1;

        // increment frequencies for prior tag
        if (isStart)
        {
            prior_freq[make_pair(pairs[i].second, pairs[i].first)] += 1;
            prior_tag_freq[pairs[i].second] += 1;
            isStart = false;
        }
    }
}

void Dataset::calculate_probs()
{
    for (auto &entry : transition_freq)
    {
        transition_probs[entry.first] = log(entry.second) - log(tag_freq[entry.first.first]);
    }

    for (auto &entry : emission_freq)
    {
        emission_probs[entry.first] = log(entry.second) - log(tag_freq[entry.first.first]);
    }

    for (auto &entry : prior_freq)
    {
        prior_probs[entry.first] = log(entry.second) - log(prior_tag_freq[entry.first.first]);
    }
}