#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdexcept>

// Базовый класс User
class User {
protected:
    std::string name_;
    int id_;
    int accessLevel_;

public:
    User(const std::string& name, int id, int accessLevel) {
        if (name.empty()) throw std::invalid_argument("Name cannot be empty");
        if (accessLevel < 0) throw std::invalid_argument("Access level cannot be negative");
        name_ = name;
        id_ = id;
        accessLevel_ = accessLevel;
    }

    // Геттеры
    std::string getName() const { return name_; }
    int getId() const { return id_; }
    int getAccessLevel() const { return accessLevel_; }

    // Сеттеры
    void setName(const std::string& name) {
        if (name.empty()) throw std::invalid_argument("Name cannot be empty");
        name_ = name;
    }
    void setAccessLevel(int accessLevel) {
        if (accessLevel < 0) throw std::invalid_argument("Access level cannot be negative");
        accessLevel_ = accessLevel;
    }

    // Виртуальный метод для полиморфизма
    virtual void displayInfo() const {
        std::cout << "User: " << name_ << ", ID: " << id_ << ", Access Level: " << accessLevel_ << std::endl;
    }

    virtual ~User() = default;
};

// Производный класс Student
class Student : public User {
private:
    std::string group_;

public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group_(group) {
        if (group.empty()) throw std::invalid_argument("Group cannot be empty");
    }

    void displayInfo() const override {
        std::cout << "Student: " << name_ << ", ID: " << id_ << ", Access Level: " << accessLevel_
            << ", Group: " << group_ << std::endl;
    }
};

// Производный класс Teacher
class Teacher : public User {
private:
    std::string department_;

public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department_(department) {
        if (department.empty()) throw std::invalid_argument("Department cannot be empty");
    }

    void displayInfo() const override {
        std::cout << "Teacher: " << name_ << ", ID: " << id_ << ", Access Level: " << accessLevel_
            << ", Department: " << department_ << std::endl;
    }
};

// Производный класс Administrator
class Administrator : public User {
private:
    std::string role_;

public:
    Administrator(const std::string& name, int id, int accessLevel, const std::string& role)
        : User(name, id, accessLevel), role_(role) {
        if (role.empty()) throw std::invalid_argument("Role cannot be empty");
    }

    void displayInfo() const override {
        std::cout << "Administrator: " << name_ << ", ID: " << id_ << ", Access Level: " << accessLevel_
            << ", Role: " << role_ << std::endl;
    }
};

// Класс Resource
class Resource {
private:
    std::string name_;
    int requiredAccessLevel_;

public:
    Resource(const std::string& name, int requiredAccessLevel) {
        if (name.empty()) throw std::invalid_argument("Resource name cannot be empty");
        if (requiredAccessLevel < 0) throw std::invalid_argument("Required access level cannot be negative");
        name_ = name;
        requiredAccessLevel_ = requiredAccessLevel;
    }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel_;
    }

    std::string getName() const { return name_; }
    int getRequiredAccessLevel() const { return requiredAccessLevel_; }
};

// Шаблонный класс AccessControlSystem
template<typename T>
class AccessControlSystem {
private:
    std::vector<std::unique_ptr<User>> users_;
    std::vector<std::unique_ptr<Resource>> resources_;

public:
    // Добавление пользователя
    void addUser(std::unique_ptr<User> user) {
        users_.push_back(std::move(user));
    }

    // Добавление ресурса
    void addResource(std::unique_ptr<Resource> resource) {
        resources_.push_back(std::move(resource));
    }

    // Проверка доступа
    void checkAccess(const User& user, const Resource& resource) const {
        if (resource.checkAccess(user)) {
            std::cout << user.getName() << " has access to " << resource.getName() << std::endl;
        }
        else {
            std::cout << user.getName() << " does not have access to " << resource.getName() << std::endl;
        }
    }

    // Вывод информации о всех пользователях
    void displayAllUsers() const {
        for (const auto& user : users_) {
            user->displayInfo();
        }
    }

    // Поиск пользователя по имени
    User* findUserByName(const std::string& name) const {
        for (const auto& user : users_) {
            if (user->getName() == name) {
                return user.get();
            }
        }
        return nullptr;
    }

    // Поиск пользователя по ID
    User* findUserById(int id) const {
        for (const auto& user : users_) {
            if (user->getId() == id) {
                return user.get();
            }
        }
        return nullptr;
    }

    // Поиск ресурса по имени
    Resource* findResourceByName(const std::string& name) const {
        for (const auto& resource : resources_) {
            if (resource->getName() == name) {
                return resource.get();
            }
        }
        return nullptr;
    }

    // Сортировка пользователей по уровню доступа
    void sortUsersByAccessLevel() {
        std::sort(users_.begin(), users_.end(),
            [](const auto& a, const auto& b) {
                return a->getAccessLevel() < b->getAccessLevel();
            });
    }

    // Сохранение данных в файл
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Unable to open file for writing");

        for (const auto& user : users_) {
            file << "User: " << user->getName() << "," << user->getId() << "," << user->getAccessLevel() << "\n";
        }
        for (const auto& resource : resources_) {
            file << "Resource: " << resource->getName() << "," << resource->getRequiredAccessLevel() << "\n";
        }
        file.close();
    }

    // Загрузка данных из файла
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Unable to open file for reading");

        users_.clear();
        resources_.clear();
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("User:") == 0) {
                // Пример: User: John Doe,1,5
                std::string name;
                int id, accessLevel;
                size_t pos1 = line.find(":") + 2;
                size_t pos2 = line.find(",");
                name = line.substr(pos1, pos2 - pos1);
                pos1 = pos2 + 1;
                pos2 = line.find(",", pos1);
                id = std::stoi(line.substr(pos1, pos2 - pos1));
                accessLevel = std::stoi(line.substr(pos2 + 1));
                users_.push_back(std::make_unique<User>(name, id, accessLevel));
            }
            else if (line.find("Resource:") == 0) {
                // Пример: Resource: Library,3
                std::string name;
                int requiredAccessLevel;
                size_t pos1 = line.find(":") + 2;
                size_t pos2 = line.find(",");
                name = line.substr(pos1, pos2 - pos1);
                requiredAccessLevel = std::stoi(line.substr(pos2 + 1));
                resources_.push_back(std::make_unique<Resource>(name, requiredAccessLevel));
            }
        }
        file.close();
    }
};

// Пример использования
int main() {
    try {
        AccessControlSystem<User> system;

        // Добавление пользователей
        system.addUser(std::make_unique<Student>("Jessy Pinkman", 1, 3, "CS101"));
        system.addUser(std::make_unique<Teacher>("Walter White", 2, 7, "Chemistry Science"));
        system.addUser(std::make_unique<Administrator>("Soul Goodman", 3, 10, "System Admin"));

        // Добавление ресурсов
        system.addResource(std::make_unique<Resource>("Library", 2));
        system.addResource(std::make_unique<Resource>("Lab", 5));
        system.addResource(std::make_unique<Resource>("Server Room", 8));

        // Вывод информации о пользователях
        std::cout << "All users:\n";
        system.displayAllUsers();

        // Проверка доступа
        User* user = system.findUserByName("Jessy Pinkman");
        Resource* resource = system.findResourceByName("Library");
        if (user && resource) {
            system.checkAccess(*user, *resource);
        }
        else {
            std::cout << "User or resource not found\n";
        }

        // Проверка доступа для другого пользователя и ресурса
        user = system.findUserByName("Walter White");
        resource = system.findResourceByName("Server Room");
        if (user && resource) {
            system.checkAccess(*user, *resource);
        }
        else {
            std::cout << "User or resource not found\n";
        }

        // Сортировка по уровню доступа
        std::cout << "\nSorted by access level:\n";
        system.sortUsersByAccessLevel();
        system.displayAllUsers();

        // Сохранение в файл
        system.saveToFile("data.txt");

        // Загрузка из файла
        AccessControlSystem<User> newSystem;
        newSystem.loadFromFile("data.txt");
        std::cout << "\nLoaded from file:\n";
        newSystem.displayAllUsers();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}