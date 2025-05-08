#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>

class Monster {
public:
    std::string name;
    int health;
    int attack;

    Monster(const std::string& name, int health, int attack)
        : name(name), health(health), attack(attack) {}

    void displayInfo() const {
        std::cout << "Monster: " << name << ", Health: " << health << ", Attack: " << attack << std::endl;
    }
};

class Character {
public:
    std::string name;
    int health;
    int attack;

    Character(const std::string& name, int health, int attack)
        : name(name), health(health), attack(attack) {}

    void displayInfo() const {
        std::cout << "Character: " << name << ", Health: " << health << ", Attack: " << attack << std::endl;
    }
};

std::vector<Monster> monsters;
std::mutex monstersMutex;
std::mutex battleMutex;

void generateMonsters() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3)); // Новый монстр каждые 3 секунды
        std::lock_guard<std::mutex> lock(monstersMutex);
        monsters.push_back(Monster("Goblin", 50, 15));
        std::cout << "New monster generated!\n";
    }
}

void battle(Character& hero) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Интервал между боями

        std::lock_guard<std::mutex> lock(monstersMutex);
        if (!monsters.empty()) {
            Monster& monster = monsters.back(); // Берем последнего монстра

            // Бой
            while (hero.health > 0 && monster.health > 0) {
                // Персонаж атакует монстра
                monster.health -= hero.attack;
                std::cout << hero.name << " attacks " << monster.name << " for " << hero.attack << " damage!\n";

                if (monster.health <= 0) {
                    std::cout << monster.name << " has been defeated!\n";
                    monsters.pop_back(); // Удаляем монстра из списка
                    break;
                }

                // Монстр атакует персонажа
                hero.health -= monster.attack;
                std::cout << monster.name << " attacks " << hero.name << " for " << monster.attack << " damage!\n";

                if (hero.health <= 0) {
                    std::cout << hero.name << " has been defeated!\n";
                    return; // Завершаем бой
                }
            }
        }
    }
}

int main() {
    std::thread monsterGenerator(generateMonsters);
    monsterGenerator.detach(); // Отсоединяем поток генерации монстров

    Character hero("Hero", 100, 20);
    hero.displayInfo();

    std::thread battleThread(battle, std::ref(hero)); // Запускаем поток боя
    battleThread.join(); // Ждем завершения потока боя

    return 0;
}
