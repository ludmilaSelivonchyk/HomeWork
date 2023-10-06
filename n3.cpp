// поворот изображения на 90 градусов против часовой
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitDepth;
    uint32_t compression;
    uint32_t imageSize;
    int32_t horizontalResolution;
    int32_t verticalResolution;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

void rotateImage90DegreesCounterClockwise(std::vector<std::vector<uint32_t>>& image) {
    std::vector<std::vector<uint32_t>> rotatedImage(image[0].size(), std::vector<uint32_t>(image.size()));

    for (size_t i = 0; i < image.size(); i++) {
        for (size_t j = 0; j < image[i].size(); j++) {
            rotatedImage[j][image.size() - i - 1] = image[i][j];
        }
    }

    image = rotatedImage;
}

int main() {
    std::string inputImagePath = "b.bmp"; 
    std::string outputImagePath = "b3.bmp"; 


    std::ifstream inputFile(inputImagePath, std::ios::binary);
    if (!inputFile) {
        std::cout << "Не удалось открыть файл изображения!" << std::endl;
        return 1;
    }

    BMPHeader header;

    inputFile.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));


    if (header.signature != 0x4D42) {
        std::cout << "Неправильный формат изображения!" << std::endl;
        return 1;
    }

    if (header.bitDepth != 24) {
        std::cout << "Неправильная битовая глубина изображения!" << std::endl;
        return 1;
    }


    int padding = (4 - (header.width * sizeof(uint32_t)) % 4) % 4;


    std::vector<std::vector<uint32_t>> image(header.height, std::vector<uint32_t>(header.width));
    for (int i = 0; i < header.height; i++) {
        inputFile.read(reinterpret_cast<char*>(image[i].data()), header.width * sizeof(uint32_t));
        inputFile.ignore(padding);
    }


    rotateImage90DegreesCounterClockwise(image);


    std::ofstream outputFile(outputImagePath, std::ios::binary);
    if (!outputFile) {
        std::cout << "Не удалось создать файл для сохранения результата!" << std::endl;
        return 1;
    }

    outputFile.write(reinterpret_cast<const char*>(&header), sizeof(BMPHeader));


    for (int i = 0; i < header.height; i++) {
        outputFile.write(reinterpret_cast<const char*>(image[i].data()), header.width * sizeof(uint32_t));
        outputFile.write("\0\0\0", padding); 
    }

    std::cout << "Изображение успешно повернуто на 90 градусов и сохранено!" << std::endl;

    return 0;
}