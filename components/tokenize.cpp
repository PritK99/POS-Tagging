#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <unordered_map>
#include <utility>
#include <algorithm>

#include "data.hpp"

using namespace std;

/*
* Function Name: tokenize
* Input: A string 's' to be tokenized
* Output: A vector of strings representing the tokens obtained from the input string
* Logic: Tokenizes the input string by splitting it into individual words and punctuation marks. It iterates through each character in the input string, identifying word boundaries based on spaces and punctuation. Words and punctuation marks are added to the 'words' vector. The resulting vector is then returned.
* Example Call: vector<string> result = tokenize("Hello, world!");
*/
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

/*
* Function Name: preprocess
* Input: 
  - vector<string> words: A vector of words to be preprocessed
  - Dataset dataset: An instance of the Dataset class containing a vocabulary in the 'vocab' member variable
* Output: A vector of preprocessed words based on the vocabulary, replacing out-of-vocabulary words with "UNK"
* Logic: Converts each word to lowercase and checks if it is present in the vocabulary ('vocab') of the provided dataset. If the word is in the vocabulary, it is added to the 'processed_words' vector; otherwise, "UNK" is added. The resulting vector represents the preprocessed version of the input words.
* Example Call: vector<string> result = preprocess(wordVector, datasetInstance);
*/
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