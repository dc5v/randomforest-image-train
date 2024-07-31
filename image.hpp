#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "parameters.hpp"

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

void generate_train_image(const string &filename, Mat &converted_image, vector<double> &data, int x1, int y1, int x2, int y2)
{
    Mat image = imread(filename, IMREAD_GRAYSCALE);

    cout << "imread" << filename << "\n";


    if (image.empty())
    {
        cerr << "error: image file not found " << filename << "\n";
        exit(1);
    }

    GaussianBlur(image, image, Size(5, 5), 0);

    Mat convert = image.clone();
    int color = cvRound(255.0 / COLOR_LIMIT);

    for (int y = 0; y < image.rows; ++y)
    {
        for (int x = 0; x < image.cols; ++x)
        {
            int pixel = image.at<uchar>(y, x);

            pixel = (pixel / color) * color;
            convert.at<uchar>(y, x) = pixel;

            data.push_back(pixel);
        }
    }

    data.push_back(x1);
    data.push_back(y1);
    data.push_back(x2);
    data.push_back(y2);

    converted_image = convert;
}

#endif
