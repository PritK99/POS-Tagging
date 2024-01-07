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

    dataset.dp.resize(dataset.tag_to_idx.size(), vector<int>(dataset.word_to_idx.size(), 0));
    dataset.tags.resize(dataset.tag_to_idx.size(), vector<int>(dataset.word_to_idx.size(), 0));
}

// void forward_pass()
// {

// }

// vector <string> backward_pass()
// {

// }