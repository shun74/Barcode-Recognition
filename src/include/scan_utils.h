#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

namespace scan_utils
{
    void decodeLine(std::vector<int>& width_line, std::vector<int>& decoded_line, float mean);

    void getWidthList(cv::Mat& line, std::vector<int>& width_list, std::vector<int>& x_list, int val1, int val2);

    int binaryLineToNumber(std::vector<int>& binary_line);

    void fillNumberList(std::vector<int>& number_list, std::vector<int>& ret_list, int window_size);

    void getBestBarcode(std::vector<int>& number_list, int barcode_data[], int min_barcode_size);
}