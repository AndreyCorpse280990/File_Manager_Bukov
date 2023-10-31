#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;  // Для упрощения обращения к std::filesystem

class FileManager
{
public:
    void showContents(const std::string& path) {
        if (fs::exists(path) && fs::is_directory(path)) {
            std::cout << "Содержимое " << path << ":\n";
            for (const auto& entry : fs::directory_iterator(path)) {
                std::cout << entry.path().filename().u8string() << std::endl;
            }
        }
        else {
            std::cout << "Директория " << path << " не существует или не является директорией.\n";
        }
    }

    void createFolder(const std::string& path, const std::string& name) {
        fs::create_directory(path + "/" + name);
    }

    void createFile(const std::string& path, const std::string& name) {
        std::ofstream file(path + "/" + name);
        file.close();
    }

    void deleteObject(const std::string& path) {
        fs::remove_all(path);
    }

    void rename(const std::string& oldName, const std::string& newName) {
        fs::rename(oldName, newName);
    }

    // Остальные методы по копированию, перемещению, определению размера и поиску могут быть добавлены здесь
};

class File
{
    // Методы и свойства, специфичные для работы с файлами
};

class Folder
{
    // Методы и свойства, специфичные для работы с папками
};

int main()
{
    setlocale(LC_ALL, "rus");

    std::string diskPath = "";
    std::cout << "Введите имя диска. Например 'C:' - для Windows. '/mnt' - для Linux: " << std::endl;
    std::cin >> diskPath;

    FileManager fileManager;

    fileManager.showContents(diskPath); // Вызов метода для показа содержимого диска

    return 0;
}
