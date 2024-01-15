#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <limits>

#include "data.hpp"
#include "tokenize.hpp"

using namespace std;

/*
* Function Name: initialization
* Input: 
  - vector<string> words: A vector of words representing the input sequence
  - Dataset &dataset: A reference to the Dataset object that will be initialized with dynamic programming tables and tags
* Output: None
* Logic: Initializes the dynamic programming tables and tags for the first word in the input sequence. It assigns indices to POS tags, populates the 'dp' table with logarithmic prior probabilities (or a small epsilon value if not available), and initializes the 'tags' table with indices indicating the most likely previous tag for each current tag.
* Example Call: initialization(wordVector, datasetInstance);
*/
void initialization(vector <string> words, Dataset &dataset)
{
    double epsilon = numeric_limits<double>::min();
    int i = 0;

    for (auto p: dataset.POS)
    {
        dataset.tag_to_idx[p] = i;
        dataset.idx_to_tag[i] = p;
        i++;
    }

    dataset.dp.resize(dataset.tag_to_idx.size(), vector<double>(words.size()));
    dataset.tags.resize(dataset.tag_to_idx.size(), vector<int>(words.size()));

    for (int i = 0; i < dataset.POS.size(); i++)
    {
        string word = words[0];
        if (dataset.prior_probs.find(make_pair(dataset.idx_to_tag[i], word)) != dataset.prior_probs.end())
        {
            dataset.dp[i][0] = dataset.prior_probs[make_pair(dataset.idx_to_tag[i], word)];
        }
        else
        {
            dataset.dp[i][0] = log(epsilon);
        }

        dataset.tags[i][0] = 0;
    }
}

/*
* Function Name: forward_pass
* Input: 
  - vector<string> words: A vector of words representing the input sequence
  - Dataset &dataset: A reference to the Dataset object containing dynamic programming tables and probabilities
* Output: None
* Logic: Performs the forward pass of the Viterbi algorithm to calculate the maximum likelihood path for each word in the input sequence. For each column (word) and row (POS tag), it calculates the maximum probability path by considering transition probabilities, emission probabilities, and previous column probabilities. The calculated probabilities and corresponding tag indices are stored in the 'dp' and 'tags' tables, respectively.
* Example Call: forward_pass(wordVector, datasetInstance);
*/
void forward_pass(vector <string> words, Dataset &dataset)
{
    double epsilon = numeric_limits<double>::min();
    
    // i represents column
    for (int i = 1; i < dataset.dp[0].size(); i++)
    {
        // j represents row
        for (int j = 0; j < dataset.dp.size(); j++)
        {
            double max = numeric_limits<double>::lowest();
            int max_idx = -1;
            // k represents row of previous column
            for (int k = 0; k < dataset.dp.size(); k++)
            {
                double curr_transition_prob, curr_emission_prob;

                if (dataset.transition_probs.find(make_pair(dataset.idx_to_tag[j], dataset.idx_to_tag[k])) != dataset.transition_probs.end())
                {
                    curr_transition_prob = dataset.transition_probs[make_pair(dataset.idx_to_tag[j], dataset.idx_to_tag[k])];
                }
                else
                {
                    curr_transition_prob = log(epsilon);
                }

                if (dataset.emission_probs.find(make_pair(dataset.idx_to_tag[j], words[i])) != dataset.emission_probs.end())
                {
                    curr_emission_prob = dataset.emission_probs[make_pair(dataset.idx_to_tag[j], words[i])];
                }
                else
                {
                    curr_emission_prob = log(epsilon);
                }

                double temp = dataset.dp[k][i-1] + curr_transition_prob + curr_emission_prob;

                if (temp > max)
                {
                    max = temp;
                    max_idx = k;
                }
            }

            dataset.dp[j][i] = max;
            dataset.tags[j][i] = max_idx;
        }
    }
}

/*
* Function Name: backward_pass
* Input: 
  - Dataset &dataset: A reference to the Dataset object containing dynamic programming tables and tags
* Output: None
* Logic: Performs the backward pass of the Viterbi algorithm to reconstruct the most likely sequence of POS tags. It starts from the last column (word) and traces back through the 'tags' table to find the indices of the tags that maximize the overall likelihood. The resulting tag indices are converted to POS tags using the 'idx_to_tag' mapping and stored in the 'answer' member variable of the 'dataset' object.
* Example Call: backward_pass(datasetInstance);
*/
void backward_pass(Dataset &dataset)
{
    vector <int> answer_idx;

    //last column
    int c = dataset.tags[0].size() - 1;
    double max = numeric_limits<double>::lowest();
    int max_idx = 0;
    // i represents rows
    for (int i = 0; i < dataset.tags.size(); i++)
    {
        if (dataset.dp[i][c] > max)
        {
            max = dataset.dp[i][c];
            max_idx = i;
        }
    }
    answer_idx.push_back(max_idx);

    while(c > 0)
    {
        max_idx = dataset.tags[max_idx][c];
        answer_idx.push_back(max_idx);
        c--;
    }

    reverse(answer_idx.begin(), answer_idx.end());

    for (int i = 0; i < answer_idx.size(); i++)
    {
        dataset.answer.push_back(dataset.idx_to_tag[answer_idx[i]]);
    }
}