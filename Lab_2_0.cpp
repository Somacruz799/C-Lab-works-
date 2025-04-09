#include <iostream>
#include <string>

class Weapon {
private:
    std::string name;
    int damage;
    int weight;

public:
    // Конструктор
    Weapon(const std::string& n, int d, int w)
        : name(n), damage(d), weight(w) {
        std::cout << "Weapon " << name << " created!\n";
    }

    // Деструктор
    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    // Метод для вывода информации об оружии
    void displayInfo() const {
        std::cout << "Weapon: " << name << ", Damage: " << damage
            << ", Weight: " << weight << std::endl;
    }

    // Геттер для урона
    int getDamage() const {
        return damage;
    }

    // Геттер для названия
    std::string getName() const {
        return name;
    }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // Деструктор
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    // Метод для вывода информации
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Геттеры
    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getDefense() const { return defense; }

    // Сеттер для здоровья
    void setHealth(int h) {
        health = h;
        if (health < 0) health = 0;
    }
};

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
    Weapon* equippedWeapon; // Указатель на текущее экипированное оружие

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d), equippedWeapon(nullptr) {
        std::cout << "Character " << name << " created!\n";
    }

    // Деструктор
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    // Метод для экипировки оружия
    void equipWeapon(Weapon* weapon) {
        equippedWeapon = weapon;
        std::cout << name << " equips " << weapon->getName() << "!\n";
    }

    // Метод для атаки монстра с использованием текущего оружия
    void attackMonster(Monster& target) {
        if (equippedWeapon == nullptr) {
            std::cout << name << " has no weapon equipped! Using base attack.\n";
            int damage = attack - target.getDefense();
            if (damage > 0) {
                target.setHealth(target.getHealth() - damage);
                std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
            }
            else {
                std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
            }
        }
        else {
            // Урон = базовая атака персонажа + урон оружия - защита монстра
            int damage = (attack + equippedWeapon->getDamage()) - target.getDefense();
            if (damage > 0) {
                target.setHealth(target.getHealth() - damage);
                std::cout << name << " attacks " << target.getName() << " with "
                    << equippedWeapon->getName() << " for " << damage << " damage!\n";
            }
            else {
                std::cout << name << " attacks " << target.getName() << " with "
                    << equippedWeapon->getName() << ", but it has no effect!\n";
            }
        }
    }

    // Метод для вывода информации
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense;
        if (equippedWeapon != nullptr) {
            std::cout << ", Equipped Weapon: " << equippedWeapon->getName();
        }
        else {
            std::cout << ", Equipped Weapon: None";
        }
        std::cout << std::endl;
    }
};

int main() {
    // Динамическое создание персонажа и монстра
    Character* hero = new Character("Hero", 100, 20, 10);
    Monster* goblin = new Monster("Goblin", 75, 15, 5);

    // Вывод начальной информации
    std::cout << "\nInitial State:\n";
    hero->displayInfo();
    goblin->displayInfo();

    // Создание и экипировка первого оружия
    Weapon sword("Sword", 12, 5);
    std::cout << "\n";
    sword.displayInfo();
    hero->equipWeapon(&sword);
    hero->displayInfo();
    hero->attackMonster(*goblin);
    goblin->displayInfo();

    // Создание и экипировка второго оружия
    Weapon axe("Axe", 17, 8);
    std::cout << "\n";
    axe.displayInfo();
    hero->equipWeapon(&axe);
    hero->displayInfo();
    hero->attackMonster(*goblin);
    goblin->displayInfo();

    // Создание и экипировка третьего оружия
    Weapon bow("Bow", 6, 3);
    std::cout << "\n";
    bow.displayInfo();
    hero->equipWeapon(&bow);
    hero->displayInfo();
    hero->attackMonster(*goblin);
    goblin->displayInfo();

    // Не вызываем delete для hero и goblin, чтобы они не уничтожались
    // В реальном коде это приведет к утечке памяти, но для демонстрации оставляем их "живыми"

    return 0;
}