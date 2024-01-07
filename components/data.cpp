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
    unordered_map <string, int> word_to_idx;
    unordered_map <string, int> tag_to_idx;
    unordered_map <int, string> idx_to_word;
    unordered_map <int, string> idx_to_tag;
    vector<vector<double>> dp;
    vector<vector<int>> tags;

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
    pairs.push_back(make_pair("--s--", "--s--"));

    if (dataset.is_open())
    {
        cout << "Processing Dataset..." << endl;
        while (getline(dataset, s))
        {
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

    for (const auto &tag1 : POS) 
    {
        for (const auto &tag2 : POS)
        {
            if (transition_freq.find(make_pair(tag1, tag2)) != transition_freq.end())
            {
                transition_probs[make_pair(tag1, tag2)] = log((transition_freq[make_pair(tag1, tag2)] + epsilon)/ (tag_freq[tag1] + vocab.size()*epsilon));
            }
            else
            {
                transition_probs[make_pair(tag1, tag2)] = log((epsilon)/ (tag_freq[tag1] + vocab.size()*epsilon));
            }
        }
    }

    for (const auto &tag : POS) 
    {
        for (const auto &word : vocab)
        {
            if (emission_freq.find(make_pair(tag, word)) != emission_freq.end())
            {
                emission_probs[make_pair(tag, word)] = log((emission_freq[make_pair(tag, word)] + epsilon)/ (tag_freq[tag] + vocab.size()*epsilon));
            }
            else
            {
                emission_probs[make_pair(tag, word)] = log((epsilon)/ (tag_freq[tag] + vocab.size()*epsilon));
            }
        }
    }

    for (const auto &tag : POS) 
    {
        if (prior_freq.find(tag) != prior_freq.end())
        {
            prior_probs[tag] = log((prior_freq[tag] + epsilon)/ (pairs.size() + POS.size()*epsilon));
        }
        else
        {
            prior_probs[tag] = log((epsilon)/ (pairs.size() + POS.size()*epsilon));
        }
    }
}