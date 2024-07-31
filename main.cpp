#include "image.hpp"
#include "parameters.hpp"
#include "randomforest.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        cout << "usage: ./randomforestTrainImage [image_filename] [x1] [y1] [x2] [y2]\n";
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr)));

    string image_filename = argv[1];

    int x1 = atoi(argv[2]);
    int y1 = atoi(argv[3]);
    int x2 = atoi(argv[4]);
    int y2 = atoi(argv[5]);

    Mat image_convert;
    vector<double> data;

    try
    {
        generate_train_image(image_filename, image_convert, data, x1, y1, x2, y2);
    }
    catch (const runtime_error &e)
    {
        cout << e.what() << endl;
        return -1;
    }

    vector<vector<double>> dataset = { data };

    RandomForest *forest = forest_train(dataset, FOREST_TREES, FOREST_DEPTH);

    forest_savefile(forest, MODEL_FILENAME);
    free_forest(forest);

    RandomForest *loaded_forest = forest_loadfile(MODEL_FILENAME);

    if (loaded_forest != nullptr)
    {
        free_forest(loaded_forest);
    }

    return 0;
}
