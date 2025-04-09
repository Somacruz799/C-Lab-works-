#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // Приватное поле: имя персонажа
    int health;        // Приватное поле: уровень здоровья
    int maxHealth;     // Приватное поле: максимальный уровень здоровья
    int attack;        // Приватное поле: уровень атаки
    int defense;       // Приватное поле: уровень защиты

public:
    // Конструктор для инициализации данных
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), maxHealth(h), attack(a), defense(d) {}

    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }

    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.takeDamage(damage);
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

 

    //Задание 1:  Метод для лечения
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth; // Уровень здоровья не может превышать максимальное значение
        }
    }
   
    //Задание 2: Метод для получения урона
    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0; // Уровень здоровья не может быть меньше 0
        }
    }
};
   
int main() {
    // Создаем объекты персонажей
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Выводим информацию о персонажах
    hero.displayInfo();
    monster.displayInfo();

    // Герой атакует монстра
    hero.attackEnemy(monster);
    monster.displayInfo();

    // Задание 3: Герой получает урон после контратаки монстра
    hero.takeDamage(60);
    hero.displayInfo();

    // Герой лечится
    hero.heal(20);
    hero.displayInfo();

    // Проверяем лимит на количество здоровья после лечения
    hero.heal(80);
    hero.displayInfo();

    // Проверяем лимит на получение урона
    hero.takeDamage(120);
    hero.displayInfo();

    return 0;
}
