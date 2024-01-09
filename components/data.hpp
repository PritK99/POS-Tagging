#ifndef DATA_H
#define DATA_H

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

#endif