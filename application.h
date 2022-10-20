#pragma once

#include "cmd_arg_parser.h"
#include "filter_pipeline_factory.h"
#include "bitmap.h"
#include "base_filters.h"
#include "cmd_arg_parser.h"
#include "exceptions.h"

class Application {
public:
    Application();

public:
    FilterPipelineFactory fpf_;
};

void AppRun(int argc, char* argv[], Application app);
