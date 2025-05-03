#include <iostream>
#include <string>
#include <vector>

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

class Hero : public Character {
private:
    int level;

public:
    Hero(const std::string& n, int hp, int lev)
        : Character(n, hp), level(lev) {}

    void showDetails() const override {
        Character::showDetails();
        std::cout << "Level: " << level << std::endl;
    }
};

class Villain : public Character {
private:
    std::string species;

public:
    Villain(const std::string& n, int hp, const std::string& sp)
        : Character(n, hp), species(sp) {}

    void showDetails() const override {
        Character::showDetails();
        std::cout << "Species: " << species << std::endl;
    }
};

template <typename EntityType>
class GameController {
private:
    std::vector<EntityType> entityList;

public:
    void addEntity(const EntityType& entity) {
        entityList.push_back(entity);
    }

    void showAllEntities() const {
        for (const auto& entity : entityList) {
            entity->showDetails();
        }
    }
};

template <typename ItemType>
class ItemQueue {
private:
    std::vector<ItemType> items;

public:
    void enqueue(const ItemType& item) {
        items.push_back(item);
    }

    void dequeue() {
        if (!items.empty()) {
            items.erase(items.begin());
        }
        else {
            std::cout << "Queue is empty. Cannot dequeue.\n";
        }
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
    gameCtrl.addEntity(new Hero("Warrior", 120, 1));
    gameCtrl.addEntity(new Villain("Orc", 80, "Orc"));

    std::cout << "Entities in GameController:\n";
    gameCtrl.showAllEntities();

    ItemQueue<std::string> weaponQueue;
    weaponQueue.enqueue("Spear");
    weaponQueue.enqueue("Crossbow");
    weaponQueue.enqueue("Armor");
    weaponQueue.showQueue();

    weaponQueue.dequeue();
    weaponQueue.showQueue();


    return 0;
}
