#include <iostream>
#include <opencv2/opencv.hpp>

namespace data 
{
    class BarcodeData
    {
        public:
            int barcode_number_;
            cv::Point p1_;
            cv::Point p2_;

            BarcodeData()
            {
                barcode_number_ = 0;
                p1_ = cv::Point(0, 0);
                p2_ = cv::Point(0, 0);
            }

            BarcodeData(int barcode_number, cv::Point p1, cv::Point p2)
            {
                barcode_number_ = barcode_number;
                p1_ = p1;
                p2_ = p2;
            }

            int get_w()
            {
                return p2_.x - p1_.x;
            }

            int get_h()
            {
                return p2_.y - p1_.y;
            }

    };

}