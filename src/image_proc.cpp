#include <image_proc.h>

void image_proc::gammaCorrection(cv::Mat& img, cv::Mat& dst, float gamma)
{
    // Create YUV image from BGR image
    cv::Mat yuv_img;
    cv::cvtColor(img, yuv_img, cv::COLOR_BGR2YUV);
    std::vector<cv::Mat> yuv_planes;
    // Split channels of YUV iamge
    cv::split(yuv_img, yuv_planes);
    // Apply gamma correction to Y channel
    cv::Mat lut = cv::Mat(256, 1, CV_8U);
    for (int i=0; i<256; i++)
    {
        lut.data[i] = (int)(pow((float)i/255.0, gamma) * 255.0);
    }
    cv::Mat gamma_img(yuv_img.rows, yuv_img.cols, CV_8U);
    cv::LUT(yuv_planes[0], lut, gamma_img);
    // Swap Y channel of YUV image
    cv::Mat gamma_yuv_img;
    yuv_planes[0] = gamma_img;
    cv::merge(yuv_planes, gamma_yuv_img);
    // Convert YUV image to BGR image
    cv::cvtColor(gamma_yuv_img, dst, cv::COLOR_YUV2BGR);

    return;
}

void image_proc::unsharpMasking(cv::Mat& img, cv::Mat& dst, int kernel, float k)
{

    cv::Mat blur_img;

    cv::GaussianBlur(img, blur_img, cv::Size(kernel, kernel), 3);

    cv::addWeighted(img, 1.0 + k, blur_img, -k, 0, dst);

    return;
}

void image_proc::imageTransform(cv::Mat& img, cv::Mat& dst, float gamma, int kernel, float k)
{

    cv::Mat gamma_corr_img;
    
    image_proc::gammaCorrection(img, gamma_corr_img, gamma);

    image_proc::unsharpMasking(gamma_corr_img, dst, kernel, k);

    return;
}