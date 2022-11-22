#include <iostream>
#include <vector>
#include <string>

class Config {
    public:
        void setConfig(std::string config_path);

        // image proc
        bool image_process_;
        float gamma_;
        int blur_kernel_;
        float unsharp_k_;

        // barcode scan
        int num_max_barcode_;
        int barcode_length_;
        int skip_line_;
        int fill_window_size_;
        int min_barcode_size_;

        // io
        std::string output_;
        bool visualize_;
};