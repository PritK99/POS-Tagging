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

// custom hash function for using pair as a key in hashmap
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

/*
* Function Name: load_dataset
* Input: None
* Output: None
* Logic: Opens a file specified by the 'path' member variable and processes its content line by line. Each line contains a word and its corresponding part of speech. This also populates the pairs vector, which holds the word and its corresponding POS.
* Example Call: dataset.load_dataset();
*/
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

/*
* Function Name: process_line
* Input: A string 'line' representing a line from the dataset file
* Output: A pair of strings representing two words obtained from the input line
* Logic: Processes the input line to extract two words separated by a tab character ('\t'). The extracted words are then returned as a pair. If the input line is empty, a special pair ("--s--", "--s--") representing the start tag is returned.
* Example Call: pair<string, string> result = dataset.process_line("word1\tword2");
*/
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

/*
* Function Name: create_vocabulary
* Input: None
* Output: None
* Logic: Processes the pairs of strings in the 'pairs' vector member variable to create a vocabulary. It counts the frequency of each word, inserts corresponding part-of-speech (POS) tags into the 'POS' set member variable, and filters out words with a frequency less than or equal to 1. Words that meet the frequency criterion are added to the 'vocab' set member variable. If a word does not meet the frequency criterion, its corresponding word in the pair is set to "UNK". Finally, "UNK" is added to the vocabulary.
* Example Call: dataset.create_vocabulary();
*/
void Dataset::create_vocabulary()
{
    unordered_map <string, int> vocab_freq;

    for (auto p : pairs)
    {
        if (p.first == "--s--")
        {
            // do nothing for start tag
            continue;
        }
        vocab_freq[p.first]++;
        POS.insert(p.second);
    }

    for (auto p : pairs)
    {
        if (p.first == "--s--")
        {
            // do nothing for start tag
            continue;
        }
        else if (vocab_freq[p.first] > 1)
        {
            vocab.insert(p.first);
        }
        else
        {
            p.first = "UNK";
        }
        vocab_freq[p.first]++;
    }

    vocab.insert("UNK");

    cout << "There are " << vocab.size() << " Unique words in dataset" << endl;
    cout << "There are " << POS.size() << " Unique POS tags in dataset" << endl;
}

/*
* Function Name: count_frequencies
* Input: None
* Output: None
* Logic: Iterates through pairs of strings in the 'pairs' vector member variable to count frequencies for transition, emission, and prior probabilities. For each pair of consecutive words, it updates the transition frequency in the 'transition_freq' unordered_map. It also updates the emission frequency for the current word and its corresponding POS tag in the 'emission_freq' unordered_map, and increments the frequency of the POS tag in the 'tag_freq' unordered_map. Additionally, it handles the special case when the current word is preceded by the start tag "--s--", updating the prior frequency in the 'prior_freq' unordered_map and incrementing the prior tag frequency in the 'prior_tag_freq' unordered_map.
* Example Call: dataset.count_frequencies();
*/
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

/*
* Function Name: calculate_probs
* Input: None
* Output: None
* Logic: Calculates logarithmic probabilities for transition, emission, and prior probabilities based on the previously computed frequencies. For each entry in 'transition_freq', it calculates the transition probability and stores it in 'transition_probs'. Similarly, it calculates the emission probabilities for each entry in 'emission_freq' and stores them in 'emission_probs'. For the prior probabilities, it computes the logarithmic value and updates 'prior_probs' using the frequencies in 'prior_freq' and 'prior_tag_freq'.
* Example Call: dataset.calculate_probs();
*/
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