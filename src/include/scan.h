// #pragma once
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <data.h>

namespace scan
{
    data::BarcodeData scanBarcode(cv::Mat& img, int barcode_length, int skip, int fill_window_size, int min_barcode_size);

    std::vector<data::BarcodeData> scanAll(cv::Mat& img, int num_max_barcode, int barcode_length, int skip, int fill_window_size, int min_barcode_size);
}
