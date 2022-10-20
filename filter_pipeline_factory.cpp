#include "filter_pipeline_factory.h"
#include "exceptions.h"

#include <string>
#include <memory>

namespace FilterFactories {
std::unique_ptr<BaseFilter> MakeNegativeFilter(const FilterDescriptor& fd) {
    if (!fd.filter_params.empty()) {
        throw MyExcept(MyExcept::Code::NegativeParams);
    }
    return std::make_unique<NegativeFilter>();
}

std::unique_ptr<BaseFilter> MakeGrayscaleFilter(const FilterDescriptor& fd) {
    if (!fd.filter_params.empty()) {
        throw MyExcept(MyExcept::Code::GSParams);
    }
    return std::make_unique<GrayscaleFilter>();
}

std::unique_ptr<BaseFilter> MakeSharpeningFilter(const FilterDescriptor& fd) {
    if (!fd.filter_params.empty()) {
        throw MyExcept(MyExcept::Code::SharpParams);
    }
    return std::make_unique<SharpeningFilter>();
}

std::unique_ptr<BaseFilter> MakeEdgeDetectionFilter(const FilterDescriptor& fd) {
    if (fd.filter_params.size() != 1) {
        throw MyExcept(MyExcept::Code::EDPararms);
    }
    int threshold = std::stoi(static_cast<std::string>(fd.filter_params[0]));
    return std::make_unique<EdgeDetectionFilter>(threshold);
}

std::unique_ptr<BaseFilter> MakeCropFilter(const FilterDescriptor& fd) {
    if (fd.filter_params.size() != 2) {
        throw MyExcept(MyExcept::Code::CropParams);
    }
    int width = std::stoi(static_cast<std::string>(fd.filter_params[0]));
    int height = std::stoi(static_cast<std::string>(fd.filter_params[1]));
    return std::make_unique<CropFilter>(width, height);
}
}  // namespace FilterFactories

void FilterPipelineFactory::AddFilterMaker(std::string_view name, FilterFactory factory) {
    filter_to_makers_.insert({name, factory});
}

void FilterPipelineFactory::AddFilterMakers(std::initializer_list<std::pair<std::string_view, FilterFactory>> lst) {
    for (const auto& pair : lst) {
        AddFilterMaker(pair.first, pair.second);
    }
}

FilterPipelineFactory::FilterFactory FilterPipelineFactory::GetFilterMaker(std::string_view name) const {
    FilterToMakerMap::const_iterator it = filter_to_makers_.find(name);
    if (it != filter_to_makers_.end()) {
        return it->second;
    }
    return nullptr;
}

std::unique_ptr<BaseFilter> FilterPipelineFactory::CreateFilter(const FilterDescriptor& descriptor) const {
    FilterFactory gear = GetFilterMaker(descriptor.filter_name);
    if (!gear) {
        throw MyExcept(MyExcept::Code::CantParse);
    }
    std::unique_ptr<BaseFilter> current_filter = gear(descriptor);
    return current_filter;
}
