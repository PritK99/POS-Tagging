#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <unordered_map>
#include <utility>
#include <algorithm>

#include "data.hpp"

using namespace std;

vector<string> tokenize(string s)
{
    vector<string> words;
    string temp = "";

    for (char c : s) 
    {
        if (c == ' ' || ispunct(c)) 
        {
            if (!temp.empty()) 
            {
                words.push_back(temp);
                temp.clear();
            }
            if (ispunct(c)) 
            {
                words.push_back(string(1, c));
            }
        } 
        else 
        {
            temp += c;
        }
    }

    if (!temp.empty()) 
    {
        words.push_back(temp);
    }

    return words;
}

vector<string> preprocess(vector<string> words, Dataset dataset)
{
    vector<string> processed_words;
    for (string word : words)
    {
        for (char &c : word) 
        {
            c = tolower(c);
        }

        if (dataset.vocab.find(word) != dataset.vocab.end())
        {
            processed_words.push_back(word);
        }
        else
        {
            processed_words.push_back("UNK");
        }
    }

    return processed_words;
}