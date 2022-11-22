#include <chrono>
#include <image_proc.h>
#include <scan.h>
#include <config.h>

using namespace std;

int main(int argc, char *argv[]) {

    // Argument check
    cv::Mat img;
    string config_path;
    string image_path = argv[2];
    if (argc != 3) {
        cout << "Invalid argument. \"config_path\" and \"image_path\" are needed." << endl;
        return 0;
    }
    else {
        config_path = argv[1];
        image_path = argv[2];
    }

    // Load configs
    Config config;
    config.setConfig(config_path);

    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();

    // Image load & process
    img = cv::imread(image_path, cv::IMREAD_COLOR);
    cv::Mat processed_img;
    if (config.image_process_) {
        image_proc::imageTransform(img, processed_img, config.gamma_, config.blur_kernel_, config.unsharp_k_);
    }
    else processed_img = img;

    // Scan
    vector<data::BarcodeData> result = scan::scanAll(processed_img, config.num_max_barcode_, config.barcode_length_, config.skip_line_, config.fill_window_size_, config.min_barcode_size_);

    // Elapsed time
    end = chrono::system_clock::now();
    double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    cout << "Elapsed time: " << time << "[ms]" << endl;

    for (int i=0; i<result.size(); i++) {
        cv::rectangle(img, result[i].p1_, result[i].p2_, cv::Scalar(0, 255, 0), 3);
        cv::putText(img, to_string(result[i].barcode_number_), cv::Point(result[i].p1_.x,result[i].p1_.y-10), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255,0,0), 3 );
    }
    
    // Save result
    int path_i = image_path.find_last_of("/");
    int ext_i = image_path.find_last_of(".");
    string file_name = image_path.substr(path_i, ext_i-path_i);
    cv::imwrite(config.output_ + "/" + file_name + "_result.png", img);
    
    // Show result
    if (config.visualize_) {
        cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
        cv::imshow("image", img);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    return 0;
}