#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
/* Следует разбивать программу на файлы (но правильно). Во первых, просто ради
 * организации кода. Во вторых, чтобы была возможность проводить
 * раздельную компиляцию. Стоит помещать каждый класс\структуру\объявления
 * функций в отдельный .h файл, а определения их методов в .cpp. Тогда
 * при изменении реализации какой-то функции придется перекомпили-
 * ровать только этот .cpp файл. Если определение было в заголовке,
 * то придется перекомпилировать все файлы, в которые включен заголо-
 * вок (в крупном проекте это может сократить время компиляции на
 * несколько часов) */
#pragma pack(push, 1)
 /* А сколько из этих переменных ты на самом деле используешь?
 * А как сделать так, чтобы их стало меньше, но всё еще можно 
 * было бы считывать их одним чтением? */
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
 /* Программа хорошо ложится на использование класса картинки. То есть
 * у нас мог бы быть класс BMP, который содержит все эти методы,
 * в приватных полях хранит указатель на пиксели и BMPHeader. При 
 * изменении картинки создает новый экземпляр картинки. Тогда мы бы 
 * имели более безопасную программу за счет инкапсуляции. */
void rotateImage90DegreesClockwise(std::vector<std::vector<uint32_t>>& image) {
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
    std::string outputImagePath = "b2.bmp"; 


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


    rotateImage90DegreesClockwise(image);

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