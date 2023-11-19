#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

// Абстрактный базовый класс для файловой системы
class FileSystemObject
{
public:
    virtual void display() const = 0;  // Виртуальная функция для отображения информации
    virtual ~FileSystemObject() = default;  // Виртуальный деструктор
};

// Класс для представления файла
class File : public FileSystemObject
{
private:
    std::string name;
    std::string path;

public:
    File(const std::string& n, const std::string& p) : name(n), path(p) {}

    void display() const override
    {
        std::cout << "File: " << name << " at " << path << std::endl;
    }

    void create()
    {
        std::ofstream file(path + "/" + name);
        file.close();
        std::cout << "File created" << std::endl;
    }

    // Можно добавить другие методы для работы с файлами
};

// Класс для представления папки
class Folder : public FileSystemObject
{
private:
    std::string name;
    std::string path;

public:
    Folder(const std::string& n, const std::string& p) : name(n), path(p) {}

    void display() const override
    {
        std::cout << "Folder: " << name << " at " << path << std::endl;
    }

    void create()
    {
        fs::create_directory(path + "/" + name);
        std::cout << "Folder created" << std::endl;
    }

    // Можно добавить другие методы для работы с папками
};

// Класс для управления файловой системой
class FileManager
{
public:
    void showContents(const std::string& path)
    {
        if (fs::exists(path) && fs::is_directory(path))
        {
            std::cout << "Содержимое " << path << ":\n";
            for (const auto& entry : fs::directory_iterator(path))
            {
                std::cout << entry.path().filename().u8string() << std::endl;
            }
        }
        else
        {
            std::cout << "Директория " << path << " не существует или не является директорией.\n";
        }
    }

    void createFolder(const std::string& path, const std::string& name)
    {
        fs::create_directory(path + "/" + name);
    }

    void createFile(const std::string& path, const std::string& name)
    {
        std::ofstream file(path + "/" + name);
        file.close();
    }

    void deleteObject(const std::string& path)
    {
        fs::remove_all(path);
    }

    void rename(const std::string& oldName, const std::string& newName)
    {
        fs::rename(oldName, newName);
    }

    // Можно добавить другие методы по копированию, перемещению, определению размера и др.
};

int main()
{
    setlocale(LC_ALL, "rus");

    std::string diskPath = "";
    std::cout << "Введите имя диска. Например 'C:' - для Windows. '/mnt' - для Linux: " << std::endl;
    std::cin >> diskPath;

    FileManager fileManager;

    char choice;
    do
    {
        std::cout << "\nВыберите операцию:\n"
                  << "1. Показать содержимое директории\n"
                  << "2. Создать файл\n"
                  << "3. Создать папку\n"
                  << "4. Удалить объект\n"
                  << "5. Переименовать объект\n"
                  << "0. Выход\n";
        std::cin >> choice;

        switch (choice)
        {
        case '1':
            fileManager.showContents(diskPath);
            break;
        case '2':
        {
            std::string fileName;
            std::cout << "Введите имя файла: ";
            std::cin >> fileName;
            fileManager.createFile(diskPath, fileName);
            break;
        }
        case '3':
        {
            std::string folderName;
            std::cout << "Введите имя папки: ";
            std::cin >> folderName;
            fileManager.createFolder(diskPath, folderName);
            break;
        }
        case '4':
            fileManager.deleteObject(diskPath);
            break;
        case '5':
        {
            std::string oldName, newName;
            std::cout << "Введите старое имя: ";
            std::cin >> oldName;
            std::cout << "Введите новое имя: ";
            std::cin >> newName;
            fileManager.rename(diskPath + "/" + oldName, diskPath + "/" + newName);
            break;
        }
        case '0':
            std::cout << "Выход из программы.\n";
            break;
        default:
            std::cout << "Некорректный выбор. Попробуйте еще раз.\n";
        }

    } while (choice != '0');

    return 0;
}
