#pragma once
#include "bitmap.h"

class BaseFilter {
public:
    virtual void Apply(Bitmap& bitmap) = 0;

    virtual ~BaseFilter(){};
};

class GrayscaleFilter : public BaseFilter {
public:
    void Apply(Bitmap& bitmap) override;

    ~GrayscaleFilter(){};
};

class NegativeFilter : public BaseFilter {
public:
    void Apply(Bitmap& bitmap) override;

    ~NegativeFilter(){};
};

class SharpeningFilter : public BaseFilter {
public:
    void Apply(Bitmap& bitmap) override;

    ~SharpeningFilter(){};
};

class EdgeDetectionFilter : public GrayscaleFilter {
public:
    void Apply(Bitmap& bitmap) override;

    EdgeDetectionFilter(int threshold) : threshold_(threshold){};

    ~EdgeDetectionFilter(){};

protected:
    int threshold_ = 0;
};

class CropFilter : public BaseFilter {
public:
    void Apply(Bitmap& bitmap) override;

    CropFilter(int width, int height) : width_(width), height_(height){};

    ~CropFilter(){};

protected:
    int width_ = 0;
    int height_ = 0;
};
