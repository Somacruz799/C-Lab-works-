#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

class Entity {
public:
    virtual ~Entity() = default;
    virtual std::string getName() const = 0;
    virtual int getHealth() const = 0;
    virtual int getLevel() const = 0;
    virtual std::string serialize() const = 0; // Для сохранения
};

class Player : public Entity {
private:
    std::string name;
    int health;
    int level;

public:
    Player(const std::string& name, int health, int level)
        : name(name), health(health), level(level) {}

    std::string getName() const override { return name; }
    int getHealth() const override { return health; }
    int getLevel() const override { return level; }

    std::string serialize() const override {
        return name + " " + std::to_string(health) + " " + std::to_string(level);
    }

    static Player deserialize(const std::string& data) {
        std::istringstream iss(data);
        std::string name;
        int health, level;
        iss >> name >> health >> level;
        return Player(name, health, level);
    }
};

template<typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    ~GameManager() {
        for (T entity : entities) {
            delete entity;
        }
    }

    void addEntity(T entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const T& entity : entities) {
            std::cout << "Name: " << entity->getName()
                << ", Health: " << entity->getHealth()
                << ", Level: " << entity->getLevel() << std::endl;
        }
    }

    const std::vector<T>& getEntities() const {
        return entities;
    }
};

void saveToFile(const GameManager<Entity*>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing.");
    }

    // Запись данных в файл
    for (const Entity* entity : manager.getEntities()) {
        file << entity->serialize() << std::endl;
    }
}

void loadFromFile(GameManager<Entity*>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading.");
    }

    // Чтение данных из файла
    std::string line;
    while (std::getline(file, line)) {
        Player player = Player::deserialize(line);
        manager.addEntity(new Player(player)); // Создаем новый объект Player
    }
}

int main() {
    GameManager<Entity*> manager;
    manager.addEntity(new Player("Hero", 100, 1));
    manager.addEntity(new Player("Gargoyle", 80, 2));
    manager.addEntity(new Player("Archer", 60, 1));

    saveToFile(manager, "game_save.txt");

    GameManager<Entity*> loadedManager;
    loadFromFile(loadedManager, "game_save.txt");
    loadedManager.displayAll();

    return 0;
}
