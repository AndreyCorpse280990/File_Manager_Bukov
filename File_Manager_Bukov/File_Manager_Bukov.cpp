#include <iostream>
#include <iterator>
#include <string>
#include <filesystem>
#include <fstream>
#include <regex>
#include <windows.h> 

namespace fs = std::filesystem;

class Path
{
protected:
    fs::path currentPath;

public:
    // Метод для установки текущего пути
    void setCurrentPath(const std::string& path)
    {
        currentPath = path;
    }


    // Метод для получения текущего пути
    std::string getCurrentPath() const
    {
        return currentPath.string();
    }

    // Метод для создания папки
    void createFolder(const std::string& name)
    {
        fs::create_directory(currentPath.string() + "/" + name);
    }
};

class DiskManager : public Path
{
public:
    // Вывод списка дисков
    void showAllDrives();

    // Выбор диска
    std::string getValidDiskPath();

    // Метод для смены диска
    void changeDisk();

};

class FileManager : public DiskManager
{
public:
    // Метод для отображения содержимого директории
    void showContents(bool showSizes = true, const std::string& mask = "");

    // Метод для создания файла
    void createFile(const std::string& name);

    // Метод для удаления объекта (файла или папки)
    void deleteObject(const std::string& name);

    // Метод для переименования объекта (файла или папки)
    void rename(const std::string& oldName, const std::string& newName);

    // Метод для чтения текстового файла
    void readTextFile(const std::string& fileName);

    // Метод для перехода в директорию
    void navigateToDirectory(const std::string& directoryName);

    // Метод для перехода на уровень выше
    void navigateUp();

    // Метод для вычисления размера папки в гигабайтах
    double calculateFolderSizeGB(const fs::path& folderPath);

    // Метод для поиска файлов по маске
    void searchByMask();

    // Метод для поиска файлов по маске в подпапках
    void searchByMaskInSubfolders();

    // Вспомогательная функция для проверки соответствия строки маске
    bool matchMask(const std::string& str, const std::string& mask);

};

int main()
{
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FileManager fileManager;
    fileManager.showAllDrives();
    std::string diskPath = fileManager.getValidDiskPath();

    fileManager.setCurrentPath(diskPath);

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
            << "8. Перейти в директорию\n"
            << "9. Вернуться в предыдущую директорию\n"
            << "A. Поиск по маске\n"
            << "B. Поиск по маске во всех подпапках\n"
            << "D. Сменить диск\n"
            << "0. Выход\n"
            << "Текущая директория: " << fileManager.getCurrentPath() << std::endl;
        std::cin >> choice;

        switch (choice)
        {
        case '1':
            fileManager.showContents(false);
            break;
        case '2':
            fileManager.showContents(true);
            break;
        case '3':
        {
            std::string fileName;
            std::cout << "Введите имя файла: ";
            std::cin >> fileName;
            fileManager.createFile(fileName);
            break;
        }
        case '4':
        {
            std::string folderName;
            std::cout << "Введите имя папки: ";
            std::cin >> folderName;
            fileManager.createFolder(folderName);
            break;
        }
        case '5':
        {
            std::string objectName;
            std::cout << "Введите имя объекта для удаления: ";
            std::cin >> objectName;
            fileManager.deleteObject(objectName);
            break;
        }
        case '6':
        {
            std::string oldName, newName;
            std::cout << "Введите старое имя: ";
            std::cin >> oldName;
            std::cout << "Введите новое имя: ";
            std::cin >> newName;
            fileManager.rename(oldName, newName);
            break;
        }
        case '7':
        {
            std::string fileName;
            std::cout << "Введите имя файла для просмотра: ";
            std::cin >> fileName;
            fileManager.readTextFile(fileName);
            break;
        }
        case '8':
        {
            std::string directoryName;
            std::cout << "Введите имя директории для перехода: ";
            std::cin >> directoryName;
            fileManager.navigateToDirectory(directoryName);
            break;
        }
        case '9':
            fileManager.navigateUp();
            break;
        case 'A':
            fileManager.searchByMask();
            break;
        case 'B':
            fileManager.searchByMaskInSubfolders();
            break;
        case 'D':
            fileManager.showAllDrives();
            fileManager.changeDisk();
            break;
        case '0':
            std::cout << "Выход из программы.\n";
            break;
        default:
            std::cout << "Некорректный выбор. Попробуйте еще раз.\n";
        }

    } while (choice != '0');

    return 0;
}


void DiskManager::showAllDrives()

{
    std::vector<std::string> drives;

    //битовая маску, представляющую доступные логические диски
    DWORD drivesMask = GetLogicalDrives();
    for (char driveLetter = 'A'; driveLetter <= 'Z'; ++driveLetter)
    {
        // Проверяем, установлен ли соответствующий бит в маске
        if (drivesMask & 1)
        {
            // добавление диска в вектор
            std::string drivePath = std::string(1, driveLetter) + ":\\";
            drives.push_back(drivePath);
        }
        drivesMask >>= 1;
    }

    if (!drives.empty()) {
        std::cout << "Доступные диски:\n";
        for (const auto& drive : drives)
        {
            std::cout << drive << std::endl;
        }
    }
    else {
        std::cout << "Диски не найдены.\n";
    }
}

std::string DiskManager::getValidDiskPath()
{
    std::string diskPath;
    std::cout << "Введите имя диска. Например 'C:' - для Windows. '/mnt' - для Linux: ";
    std::cin >> diskPath;

    // проверка, существует ли указанный путь и является ли он директорией
    if (!fs::exists(diskPath) || !fs::is_directory(diskPath)) {
        std::cerr << "Ошибка: указанный путь не существует или не является директорией." << std::endl;
        exit(1);
    }

    return diskPath;
}

void DiskManager::changeDisk()
{
    std::string newDiskPath = getValidDiskPath();
    setCurrentPath(newDiskPath);
    std::cout << "Текущий диск изменен на: " << newDiskPath << std::endl;
}

void FileManager::showContents(bool showSizes, const std::string& mask)

{
    try
    {
        if (!fs::is_empty(currentPath)) // Если папка не пуста
        {
            if (fs::exists(currentPath) && fs::is_directory(currentPath))
            {
                std::cout << "Содержимое " << currentPath << ":\n";

                for (const auto& entry : fs::directory_iterator(currentPath))
                {
                    auto entryPath = entry.path();

                    // Добавлен фильтр по маске
                    if (mask.empty() || std::regex_match(entryPath.filename().u8string(), std::regex(mask)))
                    {
                        try
                        {
                            if (fs::is_directory(entryPath))
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
                                std::cout << "Папка: " << entryPath.filename().u8string();
                            }
                            else
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
                                std::cout << "Файл: " << entryPath.filename().u8string();
                            }
                            // Отображение размера, если флаг showSizes установлен
                            if (showSizes)
                            {
                                if (fs::is_regular_file(entryPath))
                                {
                                    // Определение размер файла в байтах и конвертируем в GB, MB, KB или байты в зависимости от размера
                                    int fileSizeB = fs::file_size(entryPath);
                                    if (fileSizeB >= 1024 * 1024 * 1024)
                                    {
                                        double fileSizeGB = static_cast<double>(fileSizeB) / (1024 * 1024 * 1024);
                                        std::cout << " (Размер: " << fileSizeGB << " GB)";
                                    }
                                    else if (fileSizeB >= 1024 * 1024)
                                    {
                                        double fileSizeMB = static_cast<double>(fileSizeB) / (1024 * 1024);
                                        std::cout << " (Размер: " << fileSizeMB << " MB)";
                                    }
                                    else if (fileSizeB >= 1024)
                                    {
                                        double fileSizeKB = static_cast<double>(fileSizeB) / 1024;
                                        std::cout << " (Размер: " << fileSizeKB << " KB)";
                                    }
                                    else
                                    {
                                        std::cout << " (Размер: " << fileSizeB << " байт)";
                                    }
                                }
                                else if (fs::is_directory(entryPath))
                                {
                                    // Определение размера папки в байтах и конвертируем в GB, MB, KB или байты в зависимости от размера
                                    double folderSizeGB = calculateFolderSizeGB(entryPath);
                                    if (folderSizeGB >= 1.0)
                                    {
                                        std::cout << " (Размер: " << folderSizeGB << " GB)";
                                    }
                                    else
                                    {
                                        double folderSizeMB = folderSizeGB * 1024;
                                        if (folderSizeMB >= 1.0)
                                        {
                                            std::cout << " (Размер: " << folderSizeMB << " MB)";
                                        }
                                        else
                                        {
                                            uintmax_t folderSizeB = static_cast<uintmax_t>(folderSizeMB * 1024);
                                            std::cout << " (Размер: " << folderSizeB << " байт)";
                                        }
                                    }
                                }
                            }



                            std::cout << std::endl;
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                        }
                        catch (const std::exception& e)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
                            std::cerr << "\tОшибка при обработке файла/папки: " << e.what() << std::endl;
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
                        }
                    }
                }
            }
            else
            {
                std::cout << "\tДиректория " << currentPath << " не существует или не является директорией.\n";
            }
        }
        else
        {
            std::cerr << "Папка пустая." << std::endl;
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "\tОшибка работы с файловой системой: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\tНеобработанное исключение: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "\tНеобработанное неизвестное исключение.\n";
    }
}

void FileManager::createFile(const std::string& name)

{
    fs::path filePath = currentPath / name; // проверка на занятость имени файла

    if (!fs::exists(filePath))
    {
        std::ofstream file(filePath.string());
        file.close();
        std::cout << "Файл успешно создан.\n";
    }
    else
    {
        std::cout << "Файл с именем " << name << " уже существует. Выберите другое имя.\n";
    }
}

void FileManager::deleteObject(const std::string& name)

{
    try
    {
        //  путь к объекту, объединяя текущий путь с именем объекта
        std::string objectPath = (currentPath / name).string();

        // проверка, существует ли объект по указанному пути
        if (fs::exists(objectPath))
        {
            std::cout << "Вы уверены, что хотите удалить объект " << objectPath << "? (y/n): ";
            char confirmation;
            std::cin >> confirmation;

            if (confirmation == 'y' || confirmation == 'Y')
            {
                fs::remove_all(objectPath);
                std::cout << "Объект успешно удален.\n";
            }
            else
            {
                std::cout << "Удаление отменено.\n";
            }
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

void FileManager::rename(const std::string& oldName, const std::string& newName)

{
    try
    {
        std::string oldPath = currentPath.string() + "/" + oldName;
        std::string newPath = currentPath.string() + "/" + newName;

        // std::filesystem::rename для переименования файла
        fs::rename(oldPath, newPath);
        std::cout << "Успешно переименовано, новое имя " << newName << ".";
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

void FileManager::readTextFile(const std::string& fileName)

{
    // поиск последней точки в имени файда
    size_t dotPosition = fileName.find_last_of(".");
    // найдена ли точка в имени файла
    if (dotPosition != std::string::npos)
    {
        // получение расширение файла, начиная с символа после точки
        std::string extension = fileName.substr(dotPosition + 1);
        // является ли расширение файла "txt"
        if (extension == "txt")
        {
            std::ifstream file(currentPath.string() + "/" + fileName);
            if (file.is_open())
            {
                // вывод содержимого файла построчно
                std::string line;
                std::cout << "\nСодержимое файла " << fileName << ":\n";
                while (std::getline(file, line))
                {
                    std::cout << line << std::endl;
                }
                file.close();
            }
            else
            {
                std::cout << "Не удалось открыть файл " << fileName << " для чтения.\n";
            }
        }
        else
        {
            std::cout << "Содержимое файла " << fileName << " не .txt. Расширение: " << extension << std::endl;
        }
    }
    else
    {
        std::cout << "Расширение файла не найдено в имени " << fileName << std::endl;
    }
}

void FileManager::navigateToDirectory(const std::string& directoryName)

{
    // Формирование нового пути, объединяя текущий путь и имя целевой директории
    std::string newPath = currentPath.string() + "/" + directoryName;
    if (fs::exists(newPath) && fs::is_directory(newPath))
    {
        currentPath = newPath;
        std::cout << "Переход в директорию: " << currentPath << std::endl;
    }
    else
    {
        std::cout << "Директория " << newPath << " не существует или не является директорией.\n";
    }
}

void FileManager::navigateUp()
{
    // путь к родительской директории текущего пути
    std::string parentPath = fs::path(currentPath).parent_path().u8string();
    if (!parentPath.empty())
    {
        if (fs::exists(parentPath) && fs::is_directory(parentPath))
        {
            currentPath = parentPath;
            std::cout << "Переход в директорию: " << currentPath << std::endl;
        }
        else
        {
            std::cout << "Нельзя перейти выше. Текущая директория: " << currentPath << std::endl;
        }
    }
    else
    {
        std::cout << "Нельзя перейти выше. Текущая директория: " << currentPath << std::endl;
    }
}

double FileManager::calculateFolderSizeGB(const fs::path& folderPath)

{
    //  переменная для хранения общего размера папки в байтах.
    uintmax_t sizeBytes = 0;

    // Итерация по всем файлам и подпапкам внутри указанной директории.
    for (const auto& entry : fs::recursive_directory_iterator(folderPath))
    {
        if (fs::is_regular_file(entry))
        {
            sizeBytes += fs::file_size(entry);
        }
    }

    double sizeGB = static_cast<double>(sizeBytes) / (1024 * 1024 * 1024);
    return sizeGB;
}

void FileManager::searchByMask()

{
    try
    {
        std::string mask;
        std::cout << "Введите маску файла (например, *.txt): ";
        std::cin >> mask;
        fs::path currentPathObj(currentPath);

        // lexically_normal() для обработки символов маски
        fs::path searchPath = currentPathObj.lexically_normal();

        bool found = false;
        for (const auto& entry : fs::directory_iterator(searchPath))
        {
            // получение имя файла из пути
            std::string entryName = entry.path().filename().u8string();

            // является ли элемент обычным файлом и соответствует ли маске
            if (fs::is_regular_file(entry) && matchMask(entryName, mask))
            {
                found = true;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                std::cout << "Найден файл по маске " << mask << ":\n" << entryName << std::endl;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            }
        }

        if (!found)
        {
            std::cout << "Файлы по маске " << mask << " не найдены в директории " << currentPath << ".\n";
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "\tОшибка работы с файловой системой: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "\tНеобработанное исключение: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "\tНеобработанное неизвестное исключение.\n";
    }
}

void FileManager::searchByMaskInSubfolders()
{
    try
    {
        std::string mask;
        std::cout << "Введите маску файла (например, *.txt): ";
        std::cin >> mask;

        bool found = false;
        int errorCount = 0;  //  счетчик ошибок
        int totalCount = 0;  //  счетчик всех файлов

        for (const auto& entry : fs::recursive_directory_iterator(currentPath, fs::directory_options::skip_permission_denied))
        {
            try
            {
                std::string entryName = entry.path().filename().u8string();

                // проверка статуса файла.должен иметь права, быть обычным файлом и соответствовать маске
                if (fs::status(entry).permissions() != fs::perms::none &&
                    fs::is_regular_file(entry) &&
                    matchMask(entryName, mask))
                {
                    found = true;
                    std::cout << "Найден файл " << std::endl;
                    std::cout << "  Имя файла: " << entryName << std::endl;
                    std::cout << "  Путь: " << fs::relative(entry.path(), currentPath).u8string() << std::endl;
                    ++totalCount;  // ++ счетчик всех файлов
                }
            }
            catch (const std::filesystem::filesystem_error& fe)
            {
                // Обработка ошибок файловой системы (например, "Отказано в доступе")
                std::cerr << "Ошибка работы с файловой системой: " << fe.what() << std::endl;
                ++errorCount;  // ++ счетчик ошибок
            }
        }

        if (!found)
        {
            std::cout << "Файлы по маске " << mask << " не найдены в подпапках директории " << currentPath << ".\n";
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        std::cout << "Найдено файлов: " << totalCount << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        std::cout << "Не удалось считать из-за ошибок доступа: " << errorCount << std::endl;
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

bool FileManager::matchMask(const std::string& str, const std::string& mask)

{
    size_t i = 0, j = 0;
    while (i < str.size() && j < mask.size())
    {
        // Если текущий символ в маске - '*', поиск следующго символа '*'.
        if (mask[j] == '*')
        {
            size_t k = j + 1;
            // поиск следующего символа '*' в маске.
            while (k < mask.size() && mask[k] != '*')
                ++k;

            // длину подстроки между двумя символами '*'.
            size_t len = k - j - 1;
            if (len == 0) // Если длина равна 0, '*' в маске означает любой символ.
            {
                ++j; // переход к следующему символу в маске.
                continue;
            }
            // поиск подстроки в строке, соответствующую выделенной подстроке в маске.
            size_t pos = str.find(mask.substr(j + 1, len), i);
            if (pos == std::string::npos)
                return false;

            // обновление индексов для продолжения сравнения.
            i = pos + len;
            j = k;
        }

        // Если текущий символ в маске совпадает с текущим символом в строке или является '?',
        // далее к следующему символу в строке и маске.
        else if (mask[j] == str[i] || mask[j] == '?')
        {
            ++i;
            ++j;
        }
        else
        {
            return false;
        }
    }
    // Если достигнут конец и строки, и маски, то  true.
    return (i == str.size() && j == mask.size());
}