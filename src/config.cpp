#include <fstream>
#include <sstream>
#include <algorithm>
#include <config.h>

void Config::setConfig(std::string config_path)
{
    std::ifstream ifs(config_path);
    std::string str;

    if (ifs.fail())
    {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    int line=0;

    while (getline(ifs, str))
    {
        line++;

        if (str.length()==0 or str[0]=='#') continue;

        std::vector<std::string> block;
        std::stringstream ss{str};
        std::string s;

        while(getline(ss, s, ':'))
        {
            s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
            block.push_back(s);
        }

        if (block.size()!=2)
        {
            std::cout << "Invalid config in line: " << line << ". " << "Config file: \"" << config_path << "\"" << std::endl;
            return;
        }

        // image proc
        if (block[0] == "IMAGE_PROCESS") image_process_ = (block[1]=="1");
        else if (block[0] == "GAMMA") gamma_ = std::stof(block[1]);
        else if (block[0] == "BLUR_KERNEL") blur_kernel_ = std::stoi(block[1]);
        else if (block[0] == "UNSHARP_K") unsharp_k_ = std::stof(block[1]);
        // barcode scan
        else if (block[0] == "NUM_MAX_BARCODE") num_max_barcode_ = std::stoi(block[1]);
        else if (block[0] == "BARCODE_LENGTH") barcode_length_ = std::stoi(block[1]);
        else if (block[0] == "SKIP_LINE") skip_line_ = std::stoi(block[1]);
        else if (block[0] == "FILL_WINDOW_SIZE") fill_window_size_ = std::stoi(block[1]);
        else if (block[0] == "MIN_BARCODE_SIZE") min_barcode_size_ = std::stoi(block[1]);
        // io
        else if (block[0] == "OUTPUT") output_ = block[1];
        else if (block[0] == "VISUALIZE") visualize_ = (block[1]=="1");
        else std::cout << block[0] << " is not a config param name. Line: " << line << ". " << "Config file: \"" << config_path << "\"" << std::endl;
    }

    std::cout << "Set config finished." << std::endl;
    return;
}