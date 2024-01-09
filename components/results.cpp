#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <unordered_map>
#include <utility>
#include <algorithm>

#include "data.hpp"

using namespace std;

unordered_map<string, string> tagDictionary =
    {
        {"cc", "conjunction, coordinating"},
        {"cd", "cardinal number"},
        {"dt", "determiner"},
        {"ex", "existential there"},
        {"fw", "foreign word"},
        {"in", "conjunction, subordinating or preposition"},
        {"jj", "adjective"},
        {"jjr", "adjective, comparative"},
        {"jjs", "adjective, superlative"},
        {"ls", "list item marker"},
        {"md", "verb, modal auxillary"},
        {"nn", "noun, singular or mass"},
        {"nns", "noun, plural"},
        {"nnp", "noun, proper singular"},
        {"nnps", "noun, proper plural"},
        {"pdt", "predeterminer"},
        {"pos", "possessive ending"},
        {"prp", "pronoun, personal"},
        {"prp$", "pronoun, possessive"},
        {"rb", "adverb"},
        {"rbr", "adverb, comparative"},
        {"rbs", "adverb, superlative"},
        {"rp", "adverb, particle"},
        {"sym", "symbol"},
        {"to", "infinitival to"},
        {"uh", "interjection"},
        {"vb", "verb, base form"},
        {"vbz", "verb, 3rd person singular present"},
        {"vbp", "verb, non-3rd person singular present"},
        {"vbd", "verb, past tense"},
        {"vbn", "verb, past participle"},
        {"vbg", "gerund or present participle"},
        {"wdt", "wh-determiner"},
        {"wp", "wh-pronoun, personal"},
        {"wp$", "wh-pronoun, possessive"},
        {"wrb", "wh-adverb"},
        {".", "punctuation mark, sentence closer"},
        {",", "punctuation mark, comma"},
        {":", "punctuation mark, colon"},
        {"(", "contextual separator, left paren"},
        {")", "contextual separator, right paren"}
    };

void print_result(vector<string> words, Dataset dataset)
{
    cout << endl << "/////////////////" << endl;
    for (int i = 0; i < dataset.answer.size(); i++)
    {
        cout << words[i] << " -> " << tagDictionary[dataset.answer[i]] << endl;
    }
    cout << "/////////////////" << endl;
}

// void test(string path, Dataset dataset)
// {
//     Dataset test_dataset(path);
//     test_dataset.load_dataset();

//     for (int i = 0; i < test_dataset.pairs.size(); i++)
//     {

//     }
// }