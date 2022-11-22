#include <tbb/tbb.h>
#include <opencv2/opencv.hpp>
#include <scan_utils.h>
#include <scan.h>

void scanLine(cv::Mat& line, int line_data[], int length)
{

    std::vector<int> w_width_list;
    std::vector<int> w_x_list;
    std::vector<int> b_width_list;
    std::vector<int> b_x_list;

    scan_utils::getWidthList(line, w_width_list, w_x_list, 255, 0);
    scan_utils::getWidthList(line, b_width_list, b_x_list, 0, 255);

    std::vector<int> w_start_idx_list;
    std::vector<int> b_start_idx_list;

    if (w_width_list.size() < length || b_width_list.size()<(length+1))
    {
        line_data[0] = 0;
        line_data[1] = 0;
        line_data[2] = 0;
        return;
    }
    else 
    {
        for (int i=0; i<w_width_list.size()-length; i++)
        {
            float mean = 0;
            float _std = 0;
            for (int j=0; j<length; j++) mean += w_width_list[i+j];
            mean /= length;
            for (int j=0; j<length; j++) _std += std::pow((w_width_list[i+j]-mean), 2);
            _std = std::sqrt(_std / length);
            if (_std < w_width_list[i]*0.1) 
            {
                w_start_idx_list.push_back(i);
                if (w_x_list[0] > b_x_list[0]) b_start_idx_list.push_back(i);
                else b_start_idx_list.push_back(i-1);
            }
        }
    }

    for (int i=0; i<w_start_idx_list.size(); i++) 
    {
        int w_start_idx = w_start_idx_list[i];
        int b_start_idx = b_start_idx_list[i];
        float w_width_mean = 0;
        for (int j=0; j<length; j++) w_width_mean += w_width_list[w_start_idx+j];
        w_width_mean /= length;
        std::vector<int> width_line;
        std::vector<int> decoded_line;
        for (int j=0; j<length+1; j++) width_line.push_back(b_width_list[b_start_idx+j]);
        scan_utils::decodeLine(width_line, decoded_line, w_width_mean);
        if (decoded_line[0] != -1) 
        {
            line_data[0] = scan_utils::binaryLineToNumber(decoded_line);
            line_data[1] = b_x_list[b_start_idx];
            line_data[2] = b_x_list[b_start_idx+length] + b_width_list[b_start_idx+length] - 1;
            return;
        }
    }

    line_data[0] = 0;
    line_data[1] = 0;
    line_data[2] = 0;
    return;
};

data::BarcodeData scan::scanBarcode(cv::Mat& img, int barcode_length, int skip, int fill_window_size, int min_barcode_size)
{
    cv::Mat img_binary(img.rows, img.cols, CV_8U);
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    std::vector<int> number_list(int(img_gray.rows/skip));
    std::vector<std::pair<int, int>> x_list(int(img_gray.rows/skip));

    tbb::parallel_for(0, img_gray.rows, [&](int i)
    {
        if (i%skip != 0) return;
        cv::Mat line(1, img_gray.cols, CV_8U);
        for (int j=0; j<img_gray.cols; j++) line.data[j*line.elemSize()] = img_gray.data[i*img_gray.cols + j];
        cv::threshold(line, line, 0, 255, cv::THRESH_OTSU);

        // number, x1, x2
        int line_data[3];
        scanLine(line, line_data, barcode_length-1);

        line_data[1] = line_data[1];
        line_data[2] = line_data[2];
        number_list[i/skip] = line_data[0];
        x_list[i/skip] = std::make_pair(line_data[1], line_data[2]);
    });

    int barcode_data[3];
    std::vector<int> filled_number_list;
    scan_utils::fillNumberList(number_list, filled_number_list, fill_window_size);
    scan_utils::getBestBarcode(filled_number_list, barcode_data, int(min_barcode_size/skip));
    // y position is moved by window size
    int x1 = x_list[barcode_data[1]+fill_window_size/2].first;
    int y1 = (barcode_data[1]+fill_window_size/2) * skip;
    int x2 = x_list[barcode_data[2]-fill_window_size/2].second;
    int y2 = (barcode_data[2]-fill_window_size/2) * skip;
    // number, p1, p2
    data::BarcodeData ret_data(barcode_data[0], cv::Point(x1,y1), cv::Point(x2,y2));
    return ret_data;
};

std::vector<data::BarcodeData> scan::scanAll(cv::Mat& img, int num_max_barcode, int barcode_length, int skip, int fill_window_size, int min_barcode_size) 
{
    std::vector<data::BarcodeData> ret_data;

    cv::Mat tmp_img = img.clone();
    for (int i=0; i<num_max_barcode; i++)
    {
        data::BarcodeData result = scan::scanBarcode(tmp_img, barcode_length, skip, fill_window_size, min_barcode_size);
        
        if (result.barcode_number_==0) break;

        cv::rectangle(tmp_img, cv::Point(result.p1_.x, result.p1_.y-int(result.get_h()*0.1)), cv::Point(result.p2_.x, result.p2_.y+int(result.get_h()*0.1)), cv::Scalar(0,0,0), -1);

        ret_data.push_back(result);
    }

    return ret_data;
};