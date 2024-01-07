#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <unordered_map>
#include <utility>
#include <string>
#include <algorithm>

#include "data.hpp"

using namespace std;

vector<string> tokenize(string s);
vector<string> preprocess(vector<string> words, Dataset dataset);

#endif