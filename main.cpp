#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <unordered_map>
#include <utility>
#include <algorithm>

#include "components/data.hpp"
#include "components/tokenize.hpp"
#include "components/viterbi.hpp"
#include "components/results.hpp"

using namespace std;

int main()
{
    string path = "data/dataset.pos";
    Dataset dataset(path);
    dataset.load_dataset();
    dataset.create_vocabulary();
    dataset.count_frequencies();
    dataset.calculate_probs();

    string s = "I am the president of USA";
    vector<string> words = tokenize(s);
    words = preprocess(words, dataset);

    initialization(words, dataset);
    forward_pass(dataset);
    backward_pass(dataset);
    print_result(words, dataset);
    
    return 0;
}