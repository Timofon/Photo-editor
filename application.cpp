#include "cmd_arg_parser.h"
#include "filter_pipeline_factory.h"
#include "bitmap.h"
#include "base_filters.h"
#include "application.h"

#include <memory>

Application::Application() {
    std::initializer_list<std::pair<std::string_view, FilterPipelineFactory::FilterFactory>> filters = {
        {"-crop", FilterFactories::MakeCropFilter},
        {"-gs", FilterFactories::MakeGrayscaleFilter},
        {"-neg", FilterFactories::MakeNegativeFilter},
        {"-sharp", FilterFactories::MakeSharpeningFilter},
        {"-edge", FilterFactories::MakeEdgeDetectionFilter}};
    fpf_.AddFilterMakers(filters);
}

void AppRun(int argc, char* argv[], Application app) {
    // Парсим входные данные
    CmdLineParser parser;
    if (!parser.Parse(argc, argv)) {
        throw MyExcept(MyExcept::Code::CantParse);
    }
    Bitmap bitmap;

    // Загружаем файл
    std::string input_filename = {parser.GetInputFileName().begin(), parser.GetInputFileName().end()};
    if (!bitmap.Load(input_filename.c_str())) {
        throw MyExcept(MyExcept::Code::CantLoad);
    }
    std::vector<FilterDescriptor> filter_descs = parser.GetFilters();

    // Применяем фильтры
    for (FilterDescriptor& fd : filter_descs) {
        std::unique_ptr<BaseFilter> filter;
        try {
            filter = app.fpf_.CreateFilter(fd);
            filter->Apply(bitmap);
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }

    // Записываем результат
    std::string output_filename = {parser.GetOutputFileName().begin(), parser.GetOutputFileName().end()};
    if (!bitmap.CreateFile(output_filename.c_str())) {
        throw MyExcept(MyExcept::Code::CantWrite);
    }
}
