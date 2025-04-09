#include <iostream>
#include <string>

class Entity {
protected:
    std::string name;
    int health;

public:
    // Конструктор базового класса
    Entity(const std::string& n, int h) : name(n), health(h) {}

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }

    // Виртуальный деструктор
    virtual ~Entity() {}
};

class Player : public Entity {
private:
    int experience;

public:
    // Конструктор производного класса
    Player(const std::string& n, int h, int exp)
        : Entity(n, h), experience(exp) {
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Experience: " << experience << std::endl;
    }
};

class Enemy : public Entity {
private:
    std::string type;

public:
    // Конструктор производного класса
    Enemy(const std::string& n, int h, const std::string& t)
        : Entity(n, h), type(t) {
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Type: " << type << std::endl;
    }
};

// Задание 1: Создание класса Boss, наследующего от Enemy
class Boss : public Enemy {
private:
    std::string specialAbility;

public:
    // Конструктор класса Boss
    Boss(const std::string& n, int h, const std::string& t, const std::string& ability)
        : Enemy(n, h, t), specialAbility(ability) {
    }

    // Задание 2: Переопределение метода displayInfo
    void displayInfo() const override {
        Enemy::displayInfo();
        std::cout << "Special Ability: " << specialAbility << std::endl;
    }
};

int main() {
    // Создаем объекты игрока, врага и босса
    Player hero("Hero", 100, 0);
    Enemy monster("Goblin", 50, "Goblin");
    Boss boss("Goblin King", 666, "Goblin", "Golden Magic");

    // Выводим информацию о персонажах
    std::cout << "Player Info:" << std::endl;
    hero.displayInfo();

    std::cout << "\nEnemy Info:" << std::endl;
    monster.displayInfo();

    // Задание 3: Демонстрация работы класса Boss
    std::cout << "\nBoss Info:" << std::endl;
    boss.displayInfo();



    return 0;
}