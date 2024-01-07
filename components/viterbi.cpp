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

void initialization(vector <string> words, Dataset &dataset)
{
    int i = 0;
    for (auto p: dataset.POS)
    {
        dataset.tag_to_idx[p] = i;
        dataset.idx_to_tag[i] = p;
        i++;
    }

    i = 0;
    for (auto p: words)
    {
        dataset.word_to_idx[p] = i;
        dataset.idx_to_word[i] = p;
        i++;
    }

    dataset.dp.resize(dataset.tag_to_idx.size(), vector<double>(dataset.word_to_idx.size()));
    dataset.tags.resize(dataset.tag_to_idx.size(), vector<int>(dataset.word_to_idx.size()));

    for (int i = 0; i < dataset.tag_to_idx.size(); i++)
    {
        dataset.dp[i][0] = dataset.prior_probs[dataset.idx_to_tag[i]];
        dataset.tags[i][0] = 0;
    }
}

void forward_pass(Dataset &dataset)
{
    for (int i = 1; i < dataset.dp[0].size(); i++)
    {
        for (int j = 0; j < dataset.dp.size(); j++)
        {
            double max = numeric_limits<double>::lowest();
            int max_idx = 0;
            for (int k = 0; k < dataset.dp.size(); k++)
            {
                double temp = dataset.dp[k][i-1] + dataset.transition_probs[make_pair(dataset.idx_to_tag[j], dataset.idx_to_tag[k])] + dataset.emission_probs[make_pair(dataset.idx_to_tag[j], dataset.idx_to_word[i])];
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

// vector <string> backward_pass()
// {

// }