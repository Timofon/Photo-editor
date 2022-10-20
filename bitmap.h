#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <cstring>

class Bitmap {
public:
    struct BMPHeader {
        uint16_t signature;
        uint32_t file_size;
        uint32_t dummy;
        uint32_t bitarray_offset;
    } __attribute__((__packed__));

    struct DIBHeader {
        uint32_t dib_header_size;
        uint32_t width;   // Ширина
        uint32_t height;  // Высота
        uint64_t dummy;
        uint32_t raw_bitmap_data_size;  // (including padding)
        uint64_t dummy2;
        uint64_t dummy3;
    } __attribute__((__packed__));

    struct Pixel {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };

public:
    Pixel* GetPixel(size_t row, size_t column);

    void PushBackPixel(Pixel new_pixel) {
        storage_.push_back(new_pixel);
    };

    size_t GetHeight() const {
        return dib_header_.height;
    };

    size_t GetWidth() const {
        return dib_header_.width;
    };

    void SetHeight(int height) {
        dib_header_.height = height;
    };

    void SetWidth(int width) {
        dib_header_.width = width;
    };

    void ClearStorage() {
        storage_.clear();
    };

protected:
    BMPHeader bmp_header_;
    DIBHeader dib_header_;
    Pixel pixel;
    std::vector<Pixel> storage_;

public:
    // Загружает файл из переданного потока чтения (функция под этой как раз возвращает поток)
    bool Load(std::istream& stream);

    // Загружает файл с картинкой по имени в поток
    bool Load(const char* file_name);

    // Создаёт по имени файла bmp файл и загружает туда что-то.
    bool CreateFile(const char* file_name);

    // Мы можем не иметь права записывать что-то на диск или не иметь какой-то папки
    // поэтому делим create file в виде двух функций, по аналогии с load.
    bool CreateFile(std::ofstream& stream);
};
