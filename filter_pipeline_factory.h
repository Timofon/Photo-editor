#pragma once

#include "cmd_arg_parser.h"
#include "base_filters.h"
#include "map"

#include <memory>

namespace FilterFactories {
std::unique_ptr<BaseFilter> MakeNegativeFilter(const FilterDescriptor& fd);

std::unique_ptr<BaseFilter> MakeGrayscaleFilter(const FilterDescriptor& fd);

std::unique_ptr<BaseFilter> MakeSharpeningFilter(const FilterDescriptor& fd);

std::unique_ptr<BaseFilter> MakeEdgeDetectionFilter(const FilterDescriptor& fd);

std::unique_ptr<BaseFilter> MakeCropFilter(const FilterDescriptor& fd);
}

class FilterPipelineFactory {
public:
    using FilterFactory = std::unique_ptr<BaseFilter> (*)(const FilterDescriptor&);
    using FilterToMakerMap = std::map<std::string_view, FilterFactory>;

public:
    void AddFilterMaker(std::string_view name, FilterFactory factory);

    void AddFilterMakers(std::initializer_list<std::pair<std::string_view, FilterFactory>> lst);

    // По заданному имени фильтра возвращает указатель на функцию (factory)
    FilterFactory GetFilterMaker(std::string_view name) const;

    // Создаёт фильтр по переданному ему описанию параметров командной строки
    // пользуясь шестерёнками, добавленными ранее
    std::unique_ptr<BaseFilter> CreateFilter(const FilterDescriptor& descriptor) const;

protected:
    FilterToMakerMap filter_to_makers_;
};
