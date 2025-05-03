#include <iostream>
#include <memory>
#include <vector>
#include <string>

// Базовый класс Entity
class Entity {
public:
    virtual void displayInfo() const = 0; // Чисто виртуальная функция
    virtual ~Entity() = default; // Виртуальный деструктор
};

// Класс Player
class Player : public Entity {
private:
    std::string name;
    int health;
    int level;

public:
    Player(const std::string& name, int health, int level)
        : name(name), health(health), level(level) {}

    void displayInfo() const override {
        std::cout << "Player: " << name << ", Health: " << health << ", Level: " << level << std::endl;
    }
};

// Класс Enemy
class Enemy : public Entity {
private:
    std::string name;
    int health;
    std::string type;

public:
    Enemy(const std::string& name, int health, const std::string& type)
        : name(name), health(health), type(type) {}

    void displayInfo() const override {
        std::cout << "Enemy: " << name << ", Health: " << health << ", Type: " << type << std::endl;
    }
};

// Класс Inventory
class Inventory {
private:
    std::vector<std::string> items; // Используем вектор для динамического массива

public:
    void addItem(const std::string& item) {
        items.push_back(item); // Добавление предмета
    }

    void displayInventory() const {
        std::cout << "Inventory Items:" << std::endl;
        for (const auto& item : items) {
            std::cout << "- " << item << std::endl; // Вывод предметов
        }
    }
};

int main() {
    // Массив умных указателей на базовый класс
    std::unique_ptr<Entity> entities[] = {
        std::make_unique<Player>("Hero", 100, 1),
        std::make_unique<Enemy>("Goblin", 50, "Goblin")
    };

    // Полиморфизм и автоматическое управление памятью
    for (const auto& entity : entities) {
        entity->displayInfo();
    }

    // Создаем инвентарь и добавляем предметы
    Inventory inventory;
    inventory.addItem("Sword");
    inventory.addItem("Health Potion");
    inventory.addItem("Shield");

    // Отображаем инвентарь
    inventory.displayInventory();

    return 0;
}
