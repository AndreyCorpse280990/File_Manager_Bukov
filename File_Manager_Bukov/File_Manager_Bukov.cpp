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
    void showContents(const std::string& path, bool showSizes = true)
    {
        try
        {
            if (fs::exists(path) && fs::is_directory(path))
            {
                std::cout << "Содержимое " << path << ":\n";

                for (const auto& entry : fs::directory_iterator(path))
                {
                    auto entryPath = entry.path();

                    try
                    {
                        std::string entryType = fs::is_directory(entryPath) ? "Папка" : "Файл";
                        std::cout << entryType << ": " << entryPath.filename().u8string();

                        if (showSizes)
                        {
                            if (fs::is_regular_file(entryPath))
                            {
                                std::cout << " (Размер: " << fs::file_size(entryPath) << " байт)";
                            }
                            else if (fs::is_directory(entryPath))
                            {
                                std::cout << " (Размер: " << calculateFolderSize(entryPath) << " байт)";
                            }
                        }

                        std::cout << std::endl;
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "Ошибка при обработке файла/папки: " << e.what() << std::endl;
                    }
                }
            }
            else
            {
                std::cout << "Директория " << path << " не существует или не является директорией.\n";
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Ошибка работы с файловой системой: " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Необработанное исключение: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Необработанное неизвестное исключение.\n";
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


    void deleteDirectoryContents(const std::string& path)
    {
        for (const auto& entry : fs::directory_iterator(path))
        {
            try
            {
                fs::remove_all(entry.path());
            }
            catch (const std::exception& e)
            {
                std::cerr << "Ошибка при удалении элемента: " << e.what() << std::endl;
            }
        }
    }


    void deleteObject(const std::string& path, const std::string& name)
    {
        try
        {
            std::string objectPath = path + "/" + name;

            if (fs::exists(objectPath))
            {
                fs::remove_all(objectPath);
                std::cout << "Объект успешно удален.\n";
            }
            else
            {
                std::cout << "Объект " << objectPath << " не существует.\n";
            }
        }
        catch (const std::filesystem::filesystem_error& e)
        {
            std::cerr << "Ошибка работы с файловой системой: " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Необработанное исключение: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Необработанное неизвестное исключение.\n";
        }
    }




    void rename(const std::string& oldName, const std::string& newName)
    {
        fs::rename(oldName, newName);
    }

    void readTextFile(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (file.is_open())
        {
            std::string line;
            std::cout << "\nСодержимое файла " << filePath << ":\n";
            while (std::getline(file, line))
            {
                std::cout << line << std::endl;
            }
            file.close();
        }
        else
        {
            std::cout << "Не удалось открыть файл " << filePath << " для чтения.\n";
        }
    }

private:
    // Вспомогательный метод для расчета размера папки
    uintmax_t calculateFolderSize(const fs::path& folderPath)
    {
        uintmax_t size = 0;
        for (const auto& entry : fs::recursive_directory_iterator(folderPath))
        {
            if (fs::is_regular_file(entry))
            {
                size += fs::file_size(entry);
            }
        }
        return size;
    }
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
            << "1. Показать содержимое директории без размера\n"
            << "2. Показать содержимое директории с размером\n"
            << "3. Создать файл\n"
            << "4. Создать папку\n"
            << "5. Удалить объект\n"
            << "6. Переименовать объект\n"
            << "7. Просмотреть содержимое текстового файла\n"
            << "0. Выход\n";
        std::cin >> choice;

        switch (choice)
        {
        case '1':
            fileManager.showContents(diskPath, false);
            break;
        case '2':
            fileManager.showContents(diskPath, true);
            break;
        case '3':
        {
            std::string fileName;
            std::cout << "Введите имя файла: ";
            std::cin >> fileName;
            fileManager.createFile(diskPath, fileName);
            break;
        }
        case '4':
        {
            std::string folderName;
            std::cout << "Введите имя папки: ";
            std::cin >> folderName;
            fileManager.createFolder(diskPath, folderName);
            break;
        }
        case '5':
        {
            std::string objectName;
            std::cout << "Введите имя объекта для удаления: ";
            std::cin >> objectName;
            fileManager.deleteObject(diskPath, objectName);
            break;
        }

        case '6':
        {
            std::string oldName, newName;
            std::cout << "Введите старое имя: ";
            std::cin >> oldName;
            std::cout << "Введите новое имя: ";
            std::cin >> newName;
            fileManager.rename(diskPath + "/" + oldName, diskPath + "/" + newName);
            break;
        }
        case '7':
        {
            std::string fileName;
            std::cout << "Введите имя файла для просмотра: ";
            std::cin >> fileName;
            std::string filePath = diskPath + "/" + fileName;
            fileManager.readTextFile(filePath);
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
