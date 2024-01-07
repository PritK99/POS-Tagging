#include "components/POS.hpp"

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

    return 0;
}