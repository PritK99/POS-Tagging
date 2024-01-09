#ifndef VITERBI_H
#define VITERBI_H

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

void initialization(vector <string> words, Dataset &dataset);
void forward_pass(vector <string> words, Dataset &dataset);
void backward_pass(Dataset &dataset);

#endif