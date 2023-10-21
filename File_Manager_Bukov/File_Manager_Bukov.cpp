#include <iostream>

class File
{
private:
    std::string NameFile; // имя файла
    std::string extension; // расширение файла
    int FileSize;  // размер файла
    std::string PathFile; // путь к файлу
    std::string ContentsFile; // Содержимое файла(текстового)

};


class Folder
{
private:
    std::string NameFolder; // имя папки
    std::string PathFolder; // путь к папке
    std::string ListFolder; //Список файлов и подпапок

};


class Disk
{
private:
    std::string DiskName; // Имя диска
    std::string LiskDisk; // Содержимое диска
};

int main()
{
    setlocale(LC_ALL,"rus");
    std::cout << "Hello World!\n";
}
