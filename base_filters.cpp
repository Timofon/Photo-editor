#include "base_filters.h"
#include "bitmap.h"

#include <algorithm>
#include <vector>

void GrayscaleFilter::Apply(Bitmap& bitmap) {
    for (size_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (size_t column = 0; column < bitmap.GetWidth(); ++column) {
            uint8_t new_color = 0.299 * bitmap.GetPixel(row, column)->red +
                                0.587 * bitmap.GetPixel(row, column)->green +
                                0.114 * bitmap.GetPixel(row, column)->blue;
            bitmap.GetPixel(row, column)->red = new_color;
            bitmap.GetPixel(row, column)->green = new_color;
            bitmap.GetPixel(row, column)->blue = new_color;
        }
    }
}

void NegativeFilter::Apply(Bitmap& bitmap) {
    for (size_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (size_t column = 0; column < bitmap.GetWidth(); ++column) {
            bitmap.GetPixel(row, column)->red = 255 - bitmap.GetPixel(row, column)->red;
            bitmap.GetPixel(row, column)->green = 255 - bitmap.GetPixel(row, column)->green;
            bitmap.GetPixel(row, column)->blue = 255 - bitmap.GetPixel(row, column)->blue;
        }
    }
}

void SharpeningFilter::Apply(Bitmap& bitmap) {
    std::vector<Bitmap::Pixel> storage_copy;
    for (size_t row = 0; row < bitmap.GetWidth(); ++row) {
        for (size_t column = 0; column < bitmap.GetHeight(); ++column) {
            using CoordPair = std::pair<int, int>;
            // Записываем координаты и коэффициенты из матрицы построчно
            std::vector<CoordPair> coords = {{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}};  // first: x, second: y
            std::vector<int> coefs = {-1, -1, 5, -1, -1};
            int new_red = 0;
            int new_green = 0;
            int new_blue = 0;
            size_t index = 0;
            for (CoordPair& pair : coords) {
                size_t x =
                    std::min(std::max(0, static_cast<int>(row) + pair.first), static_cast<int>(bitmap.GetWidth()) - 1);
                size_t y = std::min(std::max(0, static_cast<int>(column) + pair.second),
                                    static_cast<int>(bitmap.GetHeight()) - 1);
                new_red += coefs[index] * bitmap.GetPixel(x, y)->red;
                new_green += coefs[index] * bitmap.GetPixel(x, y)->green;
                new_blue += coefs[index] * bitmap.GetPixel(x, y)->blue;
                ++index;
            }
            Bitmap::Pixel pixel;
            pixel.red = static_cast<uint8_t>(std::min(255, std::max(0, new_red)));
            pixel.green = static_cast<uint8_t>(std::min(255, std::max(0, new_green)));
            pixel.blue = static_cast<uint8_t>(std::min(255, std::max(0, new_blue)));
            storage_copy.push_back(pixel);
        }
    }
    size_t storage_index = 0;
    for (size_t row = 0; row < bitmap.GetWidth(); ++row) {
        for (size_t column = 0; column < bitmap.GetHeight(); ++column) {
            bitmap.GetPixel(row, column)->red = storage_copy[storage_index].red;
            bitmap.GetPixel(row, column)->green = storage_copy[storage_index].green;
            bitmap.GetPixel(row, column)->blue = storage_copy[storage_index].blue;
            ++storage_index;
        }
    }
}

void EdgeDetectionFilter::Apply(Bitmap& bitmap) {
    GrayscaleFilter::Apply(bitmap);
    std::vector<Bitmap::Pixel> storage_copy;
    for (size_t row = 0; row < bitmap.GetWidth(); ++row) {
        for (size_t column = 0; column < bitmap.GetHeight(); ++column) {
            using CoordPair = std::pair<int, int>;
            // Записываем координаты и коэффициенты из матрицы построчно
            std::vector<CoordPair> coords = {{0, -1}, {-1, 0}, {0, 0}, {1, 0}, {0, 1}};  // first: x, second: y
            std::vector<int> coefs = {-1, -1, 4, -1, -1};
            int new_red = 0;
            int new_green = 0;
            int new_blue = 0;
            size_t index = 0;
            for (CoordPair& pair : coords) {
                size_t x =
                    std::min(std::max(0, static_cast<int>(row) + pair.first), static_cast<int>(bitmap.GetWidth()) - 1);
                size_t y = std::min(std::max(0, static_cast<int>(column) + pair.second),
                                    static_cast<int>(bitmap.GetHeight()) - 1);
                new_red += coefs[index] * bitmap.GetPixel(x, y)->red;
                new_green += coefs[index] * bitmap.GetPixel(x, y)->green;
                new_blue += coefs[index] * bitmap.GetPixel(x, y)->blue;
                ++index;
            }
            Bitmap::Pixel pixel;
            pixel.red = static_cast<uint8_t>(std::min(255, std::max(0, new_red)));
            pixel.green = static_cast<uint8_t>(std::min(255, std::max(0, new_green)));
            pixel.blue = static_cast<uint8_t>(std::min(255, std::max(0, new_blue)));
            storage_copy.push_back(pixel);
        }
    }
    // Копируем из временного хранилища в storage_
    size_t storage_index = 0;
    for (size_t row = 0; row < bitmap.GetWidth(); ++row) {
        for (size_t column = 0; column < bitmap.GetHeight(); ++column) {
            bitmap.GetPixel(row, column)->red = storage_copy[storage_index].red;
            bitmap.GetPixel(row, column)->green = storage_copy[storage_index].green;
            bitmap.GetPixel(row, column)->blue = storage_copy[storage_index].blue;
            ++storage_index;
        }
    }
    // Красим в белый и черный
    for (size_t row = 0; row < bitmap.GetWidth(); ++row) {
        for (size_t column = 0; column < bitmap.GetHeight(); ++column) {
            if (bitmap.GetPixel(row, column)->red > threshold_) {
                bitmap.GetPixel(row, column)->red = 255;
                bitmap.GetPixel(row, column)->green = 255;
                bitmap.GetPixel(row, column)->blue = 255;
            } else {
                bitmap.GetPixel(row, column)->red = 0;
                bitmap.GetPixel(row, column)->green = 0;
                bitmap.GetPixel(row, column)->blue = 0;
            }
        }
    }
}

void CropFilter::Apply(Bitmap& bitmap) {
    width_ = std::min(width_, static_cast<int>(bitmap.GetWidth()));
    height_ = std::min(height_, static_cast<int>(bitmap.GetHeight()));
    std::vector<Bitmap::Pixel> storage_copy;
    for (size_t row = 0; row < bitmap.GetHeight(); ++row) {
        for (size_t column = 0; column < bitmap.GetWidth(); ++column) {
            if (static_cast<int>(column) < width_ &&
                static_cast<int>(bitmap.GetHeight()) - static_cast<int>(row) < height_) {
                storage_copy.push_back(*bitmap.GetPixel(row, column));
            }
        }
    }
    bitmap.ClearStorage();
    for (size_t storage_index = 0; static_cast<int>(storage_index) < width_ * height_; ++storage_index) {
        Bitmap::Pixel pixel;
        pixel.red = storage_copy[storage_index].red;
        pixel.green = storage_copy[storage_index].green;
        pixel.blue = storage_copy[storage_index].blue;
        bitmap.PushBackPixel(pixel);
    }
    bitmap.SetHeight(height_);
    bitmap.SetWidth(width_);
}
