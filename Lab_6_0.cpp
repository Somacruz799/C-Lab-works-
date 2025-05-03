#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// Базовый класс для персонажей
class Character {
protected:
    std::string name;
    int healthPoints;

public:
    Character(const std::string& n, int hp) : name(n), healthPoints(hp) {}

    virtual void showDetails() const {
        std::cout << "Name: " << name << ", HP: " << healthPoints << std::endl;
    }

    virtual ~Character() {}
};

// Класс для героев
class Hero : public Character {
private:
    int level;

public:
    Hero(const std::string& n, int hp, int lev) : Character(n, hp), level(lev) {}

    void showDetails() const override {
        Character::showDetails();
        std::cout << "Level: " << level << std::endl;
    }
};

// Класс для злодеев
class Villain : public Character {
private:
    std::string species;

public:
    Villain(const std::string& n, int hp, const std::string& sp) : Character(n, hp), species(sp) {}

    void showDetails() const override {
        Character::showDetails();
        std::cout << "Species: " << species << std::endl;
    }
};

// Исключение для некорректного объекта
class InvalidEntityException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid entity type. Cannot add to GameController.";
    }
};

// Контроллер игры
template <typename EntityType>
class GameController {
private:
    std::vector<EntityType> entityList;

public:
    void addEntity(const EntityType& entity) {
        if (entity == nullptr) {
            throw InvalidEntityException();
        }
        entityList.push_back(entity);
    }

    void showAllEntities() const {
        for (const auto& entity : entityList) {
            entity->showDetails();
        }
    }

    ~GameController() {
        for (auto entity : entityList) {
            delete entity; // Освобождение памяти
        }
    }
};

// Исключение для пустой очереди
class EmptyQueueException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Queue is empty. Cannot pop.";
    }
};

// Очередь предметов
template <typename ItemType>
class ItemQueue {
private:
    std::vector<ItemType> items;

public:
    void enqueue(const ItemType& item) {
        items.push_back(item);
    }

    void pop() {
        if (items.empty()) {
            throw EmptyQueueException();
        }
        items.erase(items.begin());
    }

    void showQueue() const {
        std::cout << "Current Queue:\n";
        for (const auto& item : items) {
            std::cout << item << std::endl;
        }
    }
};

int main() {
    GameController<Character*> gameCtrl;

    // Проверка добавления некорректного объекта
    try {
        gameCtrl.addEntity(new Hero("Warrior", 120, 1));
        gameCtrl.addEntity(nullptr); // Попытка добавить некорректный объект
    }
    catch (const InvalidEntityException& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "Entities in GameController:\n";
    gameCtrl.showAllEntities();

    ItemQueue<std::string> weaponQueue;
    weaponQueue.enqueue("Spear");
    weaponQueue.enqueue("Crossbow");
    weaponQueue.enqueue("Armor");
    weaponQueue.showQueue();

    // Проверка обработки исключений при удалении из очереди
    try {
        weaponQueue.pop(); // Удаляем элемент
        weaponQueue.pop(); // Удаляем еще один элемент
        weaponQueue.pop(); // Удаляем последний элемент
        weaponQueue.pop(); // Попробуем удалить из пустой очереди
    }
    catch (const EmptyQueueException& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
