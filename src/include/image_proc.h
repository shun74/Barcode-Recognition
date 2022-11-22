#include <iostream>
#include <opencv2/opencv.hpp>

namespace image_proc
{
    void gammaCorrection(cv::Mat& img, cv::Mat& dst, float gamma);

    void unsharpMasking(cv::Mat& img, cv::Mat& dst, int kernel, float k);

    void imageTransform(cv::Mat& img, cv::Mat& dst, float gamma, int kernel, float k);

}