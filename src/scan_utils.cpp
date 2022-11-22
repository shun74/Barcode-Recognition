#include <vector>
#include <scan_utils.h>
    
void scan_utils::getWidthList(cv::Mat& line, std::vector<int>& width_list, std::vector<int>& x_list, int val1, int val2)
{
    int width = 0;
    for (int i=0; i<line.cols; i++)
    {
        int val = line.data[i];
        if (val == val1)
        {
            if (width == 0) x_list.push_back(i);
            width++;
        }
        else if (val == val2)
        {
            if (width > 0)
            {
                width_list.push_back(width);
                width = 0;
            }
        }
    }
    if (width>0) width_list.push_back(width);
    return;
};

void scan_utils::decodeLine(std::vector<int>& width_line, std::vector<int>& decoded_line, float mean)
{
    for (auto val_it=width_line.begin(); val_it!=width_line.end(); val_it++)
    {
        if (mean*0.15<*val_it && *val_it<mean) decoded_line.push_back(0);
        else if (mean<=*val_it<=mean*2) decoded_line.push_back(1);
        else 
        {
            decoded_line.push_back(-1);
            return;
        } 
    }
    return;
};

int scan_utils::binaryLineToNumber(std::vector<int>& decoded_line)
{
    int sum = 0;
    for (int i=0; i<decoded_line.size(); i++)
    {
        int b = decoded_line.at(decoded_line.size() - i - 1);
        sum += pow(2, i+b);
    }
    return sum;
};

int vote(std::vector<int>& number_list)
{
    std::map<int, int> vote_map;
    for (auto itr=number_list.begin(); itr!=number_list.end(); itr++)
    {
        if (vote_map.find(*itr)!=vote_map.end()) vote_map[*itr] = 1;
        else vote_map[*itr]++;
    }
    int max_vote;
    int max_num=0;
    for (auto itr=vote_map.begin(); itr!=vote_map.end(); itr++)
    {
        if (itr->second > max_num) max_vote=itr->first;
    }
    return max_vote;
}

void scan_utils::fillNumberList(std::vector<int>& number_list, std::vector<int>& ret_list, int window_size)
{
    // window_size must be odd number
    // padding sides
    for (int i=0; i<window_size/2; i++)
    {
        number_list.insert(number_list.begin(), 0);
        number_list.push_back(0);
    }
    
    for (int i=0; i<number_list.size()-window_size; i++)
    {
        std::vector<int> vote_number_list;
        for (int j=0; j<window_size; j++) vote_number_list.push_back(number_list.at(i+j));
        ret_list.push_back(vote(vote_number_list));
    }
    return;
}
 
void scan_utils::getBestBarcode(std::vector<int>& number_list, int barcode_data[], int min_barcode_size)
{
    int max_stream = 0;
    int max_stream_num = 0;
    int num = 0;
    int stream = 0;
    int start_y = 0;
    int end_y = 0;
    for (int i=0; i<number_list.size(); i++)
    {
        int val = number_list.at(i);
        if (val != num) {
            if (stream>max_stream && num!=0)
            {
                max_stream = stream;
                max_stream_num = num;
                start_y = i - stream;
                end_y = i - 1;
            }
            if (val == 0)
            {
                num = 0;
                stream = 0;
            }
            else
            {
                num = val;
                stream = 1;
            }
        }
        else stream++;
    }

    if (max_stream > min_barcode_size)
    {
        barcode_data[0] = max_stream_num;
        barcode_data[1] = start_y;
        barcode_data[2] = end_y;
    }
    else
    {
        barcode_data[0] = 0;
        barcode_data[1] = 0;
        barcode_data[2] = 0;
    }
    return;
};