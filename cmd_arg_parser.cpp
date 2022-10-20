#include "cmd_arg_parser.h"

bool CmdLineParser::Parse(int argc, char* argv[]) {
    if (argc < MIN_PARAM_NUM) {
        return false;
    }

    input_file_name_ = argv[INPUT_FILE_NAME_POS_];
    output_file_name_ = argv[OUTPUT_FILE_NAME_POS_];

    FilterDescriptor fd;
    for (int i = 4; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (!fd.filter_name.empty()) {
                filters_.push_back(fd);
            }
            fd.filter_params.clear();
            fd.filter_name = argv[i];
        } else {
            fd.filter_params.push_back(argv[i]);
        }
    }
    if (!fd.filter_name.empty()) {
        filters_.push_back(fd);
    }
    return true;
}
