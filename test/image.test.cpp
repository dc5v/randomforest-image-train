#include "../image.hpp"
#include "../parameters.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    Mat image_convert;
    vector<double> data;

    string image_filename = argv[1];

    generate_train_image(image_filename, image_convert, data, 0, 0, 1, 1);

    imwrite("converted.jpg", image_convert);
}