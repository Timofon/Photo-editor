#pragma once

#include <string_view>
#include <vector>

struct FilterDescriptor {
    std::string_view filter_name;
    std::vector<std::string_view> filter_params;
};

class CmdLineParser {
public:
    static const int MIN_PARAM_NUM = 3;
    static const int INPUT_FILE_NAME_POS_ = 2;
    static const int OUTPUT_FILE_NAME_POS_ = 3;

public:
    bool Parse(int argc, char* argv[]);

    std::string_view GetInputFileName() const {
        return input_file_name_;
    };

    std::string_view GetOutputFileName() const {
        return output_file_name_;
    };

    std::vector<FilterDescriptor> GetFilters() const {
        return filters_;
    };

protected:
    std::string_view input_file_name_;
    std::string_view output_file_name_;
    std::vector<FilterDescriptor> filters_;
};
