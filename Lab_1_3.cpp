#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int attackPower;
    int defensePower;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defensePower(d) {}

    // Виртуальный метод для а

    // Виртуальный метод для атаки
    virtual void attack(Entity& target) {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            target.setHealth(target.getHealth() - damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    // Новый метод для лечения
    virtual void heal(int amount) {
        health += amount;
        if (health > 100) health = 100; // Ограничение максимального здоровья
        std::cout << name << " heals for " << amount << " HP! Current HP: " << health << std::endl;
    }

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defensePower << std::endl;
    }

    // Геттеры
    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttackPower() const { return attackPower; }
    int getDefense() const { return defensePower; }

    // Сеттеры
    void setHealth(int h) {
        health = h;
        if (health < 0) health = 0;
        if (health > 100) health = 100;
    }

    // Виртуальный деструктор (добавлен для корректного полиморфизма)
    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    // Переопределение метода attack
    void attack(Entity& target) override {
        int damage = getAttackPower() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 20) { // 20% шанс критического удара
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    // Переопределение метода heal
    void heal(int amount) override {
        health += amount + 5; // Персонаж получает бонус +5 к лечению
        if (health > 100) health = 100; // Ограничение максимального здоровья
        std::cout << getName() << " uses a special healing ability for " << amount << " HP + 5 bonus! Current HP: " << health << std::endl;
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defensePower << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = getAttackPower() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 30) { // 30% шанс ядовитой атаки
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defensePower << std::endl;
    }
};

// Новый класс Boss, наследующий от Monster
class Boss : public Monster {
public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {}

    // Переопределение метода attack с уникальной способностью
    void attack(Entity& target) override {
        int damage = getAttackPower() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 40) { // 40% шанс огненного удара
                damage += 10; // Дополнительный урон от огненного удара
                std::cout << "Fire Strike! ";
            }
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        std::cout << "Boss: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defensePower << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0))); // Инициализация генератора случайных чисел

    // Создание объектов
    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Monster dragon("Dragon", 150, 25, 20);
    Boss finalBoss("Fire Lord", 200, 30, 15); // Создаем босса

    // Массив указателей на базовый класс
    Entity* entities[] = { &hero, &goblin, &dragon, &finalBoss };

    // Вывод информации о всех сущностях
    std::cout << "Entities Information:\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    // Бой между персонажем и монстрами
    std::cout << "\nBattle Begins!\n";
    hero.attack(goblin);
    goblin.attack(hero);
    dragon.attack(hero);
    finalBoss.attack(hero);

    // Демонстрация лечения
    std::cout << "\nHealing Demonstration:\n";
    hero.heal(20); // Персонаж лечится с бонусом
    goblin.heal(10); // Обычное лечение для монстра
    finalBoss.heal(15); // Обычное лечение для босса (унаследовано от Monster)

    // Вывод информации после боя
    std::cout << "\nEntities Information After Battle:\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    return 0;
}