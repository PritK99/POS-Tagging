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

using namespace std;

int main()
{
    string path = "data/dataset.pos";
    Dataset dataset(path);
    dataset.load_dataset();
    dataset.create_vocabulary();
    dataset.create_dictionary();
    dataset.calculate_probs();

    string s = "I am a student.";
    vector<string> words = tokenize(s);
    words = preprocess(words, dataset);

    initialization(words, dataset);
    forward_pass(dataset);
    vector <string> answer = backward_pass(dataset);

    for (string word : answer)
    {
        cout << word << " ";
    }
    
    return 0;
}