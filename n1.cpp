// определяем количество выделяемой памяти
/* Тут всё очень странно. В программе должен быть только один main().
 * В нем ты можешь вызывать нужные функции, для выполнения задачи*/

#include <iostream>
#include <fstream>
/* Так делать не стоит из-за конфликтов имен. Пусть у тебя есть функция с именем как у функции, 
 * которая орпеделена в стд. Тогда при вызове узнать, какая из эти двух функций на самом деле 
 * вызывается будет сложно. А вот если бы функцию из стд ты вызывал std::func(), то такой проблемы
 * возникнуть не может */
using namespace std;
/* Чтение и запись должны быть отдельными функциями, а лучше методами*/
int main() {
    ifstream imageFile("3.bmp", ios::binary);
    if (!imageFile) {
        cout << "Ошибка открытия файла!" << endl;
        return 1;
    }
    /* И в другом файле у тебя есть структура для заголовка,
     * и её и следовало использовать */
    char header[54];
    imageFile.read(header, 54);
    /* int и short необязательно 4 и 2 байта. Надо использовать
     * int32_t и int16_t */
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int bitsPixel = *(short*)&header[28];
    
    int bytesPixel = bitsPixel / 8;

    int imageSize = width * height * bytesPixel;
    
    cout << "Ширина: " << width << " пикселей" << endl;
    cout << "Высота: " << height << " пикселей" << endl;
    cout << "Глубина цвета: " << bitsPixel << " бит" << endl;
    cout << "Выделяемая память: " << imageSize << " байт" << endl;
    
    imageFile.close();
    
    return 0;
}
