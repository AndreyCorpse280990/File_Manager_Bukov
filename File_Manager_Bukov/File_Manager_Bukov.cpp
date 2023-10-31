#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

/* Базовый класс для файлов и папок */
class FileSystemObject {
public:
    std::string getName() const { return name; }
    std::string getPath() const { return path; }

protected:
    std::string name;
    std::string path;
};

/* Класс для работы с файлами */
class File : public FileSystemObject {
public:
    // Дополнительные методы для работы с файлами
    std::string readContent() const {
        // Реализация чтения содержимого файла
        return "File Content";
    }
};

/* Класс для работы с папками */
class Folder : public FileSystemObject {
public:
    // Дополнительные методы для работы с папками
    std::vector<FileSystemObject> getContents() const {
        std::vector<FileSystemObject> contents;
        for (const auto& entry : fs::directory_iterator(path)) {
            FileSystemObject obj;
            obj.name = entry.path().filename().u8string();
            obj.path = entry.path().u8string();
            contents.push_back(obj);
        }
        return contents;
    }
};

/* Главный класс для управления файлами и папками */
class FileManager {
public:
    void showContents(const std::string& path) {
        Folder folder;
        folder.path = path;

        if (fs::exists(path) && fs::is_directory(path)) {
            std::cout << "Содержимое " << path << ":" << std::endl;
            for (const auto& obj : folder.getContents()) {
                std::cout << obj.getName() << std::endl;
            }
        }
        else {
            std::cout << "Путь " << path << " не существует или не является директорией." << std::endl;
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
        if (fs::exists(path)) {
            if (fs::is_directory(path)) {
                fs::remove_all(path);
            }
            else {
                fs::remove(path);
            }
        }
    }

    // Другие методы для управления файлами и папками

    // Методы для других операций, таких как переименование, копирование, перемещение, вычисление размера, поиск и т.д.
};

int main() {
    FileManager fileManager;

    // Пример использования методов FileManager
    fileManager.showContents("C:/"); // Показать содержимое диска C:
    fileManager.createFolder("C:/", "NewFolder"); // Создать папку "NewFolder" на диске C:
    fileManager.createFile("C:/", "NewFile.txt"); // Создать файл "NewFile.txt" на диске C:
    fileManager.deleteObject("C:/NewFile.txt"); // Удалить файл "NewFile.txt" на диске C:

    return 0;
}
