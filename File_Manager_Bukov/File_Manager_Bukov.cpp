#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;  // Для упрощения обращения к std::filesystem

// Класс для описания файла
class File 
{
private:
    std::string NameFile; // имя файла
    std::string extension; // расширение файла
    int FileSize;  // размер файла
    std::string PathFile; // путь к файлу
    std::string ContentsFile; // Содержимое файла(текстового)

public:
    // Конструктор c параметрами
    File(const std::string& name, const std::string& extension, int fileSize, const std::string& path, const std::string& contents = "");

    // Метод для отображения информаии о файле
    void DisplayInfo();
};
// Реализация конструктора c параметрами
File::File(const std::string& name, const std::string& extension, int fileSize, const std::string& path, const std::string& contents)
    : NameFile(name), extension(extension), FileSize(fileSize), PathFile(path), ContentsFile(contents) {
    std::cout << "Конструктор File по умолчанию отработал" << ", по адресу " << this << std::endl;
}

// Реализация метода для отображения информации о файле
void File::DisplayInfo() {
    std::cout << "Имя файла: " << NameFile << std::endl;
    std::cout << "Расширение файла: " << extension << std::endl;
    std::cout << "Размер файла: " << FileSize << " байт" << std::endl;
    std::cout << "Путь к файлу: " << PathFile << std::endl;
}

// Класс для описания папки
class Folder {
private:
    std::string NameFolder; // имя папки
    std::string PathFolder; // путь к папке

public:
    // Конструктор c параметрами
    Folder(const std::string& name, const std::string& path) : NameFolder(name), PathFolder(path) {}

    // Метод для отображения информации о папке
    void DisplayInfo() {
        std::cout << "Имя папки: " << NameFolder << std::endl;
        std::cout << "Путь к папке: " << PathFolder << std::endl;
    }
};

// Класс для описания диска
class Disk 
{
private:
    std::string DiskName; // Имя диска

public:
    // Конструктор c параметрами
    Disk(const std::string& name) : DiskName(name) {}

    // Метод для отображения информации о диске
    void DisplayInfo() 
    {
        std::cout << "Имя диска: " << DiskName << std::endl;
    }
};

// Класс для описания текущей директории
class CurrentDirectory 
{
private:
    std::string path; // Текущая директория

public:
    // Конструктор для инициализации текущей директории
    CurrentDirectory(const std::string& initialPath) : path(initialPath) {}

    // Метод для отображения текущей директории
    void DisplayCurrentDirectory() {
        std::cout << "Текущая директория: " << path << std::endl;
    }
};


int setDisk(std::string diskPath) 
{
        // Проверим, существует ли диск
    if (fs::exists(diskPath) && fs::is_directory(diskPath)) 
    {
        std::cout << "Содержимое диска " << diskPath << ":" << std::endl;

        for (const auto& entry : fs::directory_iterator(diskPath)) {
            std::cout << entry.path().filename().u8string() << std::endl;
        }
    } else 
    {
        std::cout << "Диск " << diskPath << " не существует или не является директорией." << std::endl;
        
    }
}

int setFile(std::string filePath)
{   
    std::string extension = filePath.extension().string();

    if(extension == ".txt" || extension == "csv")
    {
        std::cout << "Выбран файл " << filePath << std::endl;
    }
}
int main() {
    setlocale(LC_ALL, "rus");

    // Укажите букву диска, который вы хотите исследовать (например, "C:")
    std::string diskPath = "/media/andrey";
    std::string filePath = "/media/andrey/test.txt";
    setDisk(diskPath);

    
    return 0;
}
