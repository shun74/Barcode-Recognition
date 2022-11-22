#include <chrono>
#include <image_proc.h>
#include <scan.h>
#include <config.h>

using namespace std;

int main(int argc, char *argv[]) {

    string config_path;

    if (argc != 2) {
        cout << "Invalid argument. \"config_path\" is needed." << endl;
        return 0;
    }
    else {
        config_path = argv[1];
    }

    Config config;
    config.setConfig(config_path);

    cv::VideoCapture cap;
    cap.open(0, cv::CAP_V4L2);
    if (cap.isOpened() == false) {
        cout << "Video file doesn't open !" << endl;
        return 0;
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 30);
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));


    // Scan
    cv::Mat img;
    cv::Mat processed_img;
    vector<data::BarcodeData> result;

    while (cap.read(img)) {
        if (img.empty() == true) {
            cout << "empty" << endl;
            break;
        }
        
        if (config.image_process_) image_proc::imageTransform(img, processed_img, config.gamma_, config.blur_kernel_, config.unsharp_k_);
        else processed_img = img;

        result = scan::scanAll(processed_img, config.num_max_barcode_, config.barcode_length_, config.skip_line_, config.fill_window_size_, config.min_barcode_size_);
    
        for (int i=0; i<result.size(); i++) {
            cv::rectangle(img, result[i].p1_, result[i].p2_, cv::Scalar(0, 255, 0), 3);
            cv::putText(img, to_string(result[i].barcode_number_), cv::Point(result[i].p1_.x,result[i].p1_.y-10), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255,0,0), 3 );
        }
        
        cv::imshow("image", img);
        if (cv::waitKey(1) == 'q') break;

    }
    cv::destroyAllWindows();

    return 0;
}