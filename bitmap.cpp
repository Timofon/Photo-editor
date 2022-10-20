#include "bitmap.h"

#include <fstream>

bool Bitmap::Load(const char* file_name) {
    std::fstream file;
    file.open(file_name, std::ios_base::in | std::ios_base::binary);  // Открываем файл на чтение как бинарный
    if (!file.is_open()) {
        return false;
    }
    Load(file);
    return true;
}

bool Bitmap::Load(std::istream& stream) {
    stream.read(reinterpret_cast<char*>(&bmp_header_), sizeof(bmp_header_));
    stream.read(reinterpret_cast<char*>(&dib_header_), sizeof(dib_header_));
    for (size_t row = 0; row < dib_header_.width; ++row) {
        for (size_t column = 0; column < dib_header_.height; ++column) {
            stream.read(reinterpret_cast<char*>(&pixel), sizeof(pixel));
            storage_.push_back(pixel);
        }
    }
    return true;
}

bool Bitmap::CreateFile(const char* file_name) {
    std::ofstream file;
    file.open(file_name, std::ios_base::out | std::ios_base::binary);  // Открываем файл на запись как бинарный
    CreateFile(file);
    return true;
}

bool Bitmap::CreateFile(std::ofstream& stream) {
    stream.write(reinterpret_cast<char*>(&bmp_header_), sizeof(bmp_header_));
    stream.write(reinterpret_cast<char*>(&dib_header_), sizeof(dib_header_));
    for (size_t i = 0; i < storage_.size(); ++i) {
        stream.write(reinterpret_cast<char*>(&storage_[i]), sizeof(storage_[i]));
    }
    return true;
}

Bitmap::Pixel* Bitmap::GetPixel(size_t row, size_t column) {
    return &storage_[dib_header_.width * row + column];
}
