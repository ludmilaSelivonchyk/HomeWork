// определяем количество выделяемой памяти


#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream imageFile("3.bmp", ios::binary);
    if (!imageFile) {
        cout << "Ошибка открытия файла!" << endl;
        return 1;
    }
    
    char header[54];
    imageFile.read(header, 54);
    
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
