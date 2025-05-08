#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <ctime>
#include <limits>

// Template Logger class for logging game events
template <typename T>
class Logger {
private:
    std::string log_filename;

public:
    Logger(const std::string& fname) : log_filename(fname) {
        std::ofstream file(log_filename, std::ios::app);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open log file: " + log_filename);
        }
        file << "Log initiated at " << std::time(nullptr) << "\n";
    }

    void log(const T& event) {
        std::ofstream file(log_filename, std::ios::app);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to append to log file: " + log_filename);
        }
        file << event << "\n";
    }
};

// Inventory class
class Inventory {
private:
    std::vector<std::string> items_list;

public:
    void addItem(const std::string& item) {
        if (item.empty()) {
            throw std::invalid_argument("Item name must not be empty.");
        }
        items_list.push_back(item);
        std::cout << "Added " << item << " to inventory.\n";
    }

    void removeItem(const std::string& item) {
        for (auto it = items_list.begin(); it != items_list.end(); ++it) {
            if (*it == item) {
                items_list.erase(it);
                std::cout << "Removed " << item << " from inventory.\n";
                return;
            }
        }
        throw std::invalid_argument("Item " + item + " not found in inventory.");
    }

    void displayInventory() const {
        if (items_list.empty()) {
            std::cout << "Inventory is empty.\n";
            return;
        }
        std::cout << "Inventory Contents:\n";
        for (const auto& item : items_list) {
            std::cout << "- " << item << "\n";
        }
    }

    std::string serialize() const {
        std::stringstream ss;
        ss << items_list.size();
        for (const auto& item : items_list) {
            ss << " " << item;
        }
        return ss.str();
    }

    void deserialize(const std::string& data) {
        std::istringstream iss(data);
        size_t count;
        iss >> count;
        items_list.clear();
        std::string item;
        std::getline(iss, item); // Consume whitespace
        for (size_t i = 0; i < count; ++i) {
            std::getline(iss, item, ' ');
            if (!item.empty()) {
                items_list.push_back(item);
            }
        }
    }
};

// Base Monster class
class Monster {
protected:
    std::string name;
    int health_points;
    int attack_power;
    int defense_value;

public:
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health_points(h), attack_power(a), defense_value(d) {
        validate();
    }

    virtual void displayInfo() const {
        std::cout << "Monster: " << name << ", HP: " << health_points
            << ", Attack: " << attack_power << ", Defense: " << defense_value << std::endl;
    }

    virtual std::string getType() const = 0;
    virtual std::string serialize() const {
        return getType() + "," + name + "," + std::to_string(health_points) + "," +
            std::to_string(attack_power) + "," + std::to_string(defense_value);
    }

    virtual ~Monster() = default;

    std::string getName() const { return name; }
    int getHealth() const { return health_points; }
    int getAttack() const { return attack_power; }
    int getDefense() const { return defense_value; }

    void takeDamage(int damage, Logger<std::string>& logger) {
        if (damage < 0) {
            throw std::invalid_argument("Damage must be non-negative.");
        }
        health_points -= damage;
        if (health_points < 0) {
            health_points = 0;
            logger.log(name + " has been defeated!");
            throw std::runtime_error("Health dropped below zero for " + name);
        }
        logger.log(name + " takes " + std::to_string(damage) + " damage, HP now " + std::to_string(health_points));
    }

protected:
    void validate() const {
        if (health_points <= 0) throw std::invalid_argument("Health must be positive.");
        if (name.empty()) throw std::invalid_argument("Name must not be empty.");
        if (attack_power < 0 || defense_value < 0) throw std::invalid_argument("Attack and defense must be non-negative.");
    }
};

// Derived Monster classes
class Skeleton : public Monster {
public:
    Skeleton(const std::string& n, int h = 40, int a = 10, int d = 15)
        : Monster(n, h, a, d) {
    }

    std::string getType() const override { return "Skeleton"; }
};

class Lich : public Monster {
public:
    Lich(const std::string& n, int h = 600, int a = 35, int d = 25)
        : Monster(n, h, a, d) {
    }

    std::string getType() const override { return "Lich"; }
};

// Character class
class Character {
private:
    std::string character_name;
    int health_points;
    int attack_power;
    int defense_value;
    int character_level;
    int exp_points;
    Inventory inventory;

public:
    Character(const std::string& n, int h, int a, int d)
        : character_name(n), health_points(h), attack_power(a), defense_value(d), character_level(1), exp_points(0) {
        validate();
        inventory.addItem("Sword");
        inventory.addItem("Shield");
    }

    void attackEnemy(Monster& enemy, Logger<std::string>& logger) {
        int damage = attack_power - enemy.getDefense();
        if (damage > 0) {
            enemy.takeDamage(damage, logger);
            logger.log(character_name + " attacks " + enemy.getName() + " for " + std::to_string(damage) + " damage!");
        }
        else {
            logger.log(character_name + " attacks " + enemy.getName() + ", but it has no effect!");
        }
    }

    void takeDamage(int damage, Logger<std::string>& logger) {
        if (damage < 0) {
            throw std::invalid_argument("Damage must be non-negative.");
        }
        health_points -= damage;
        if (health_points < 0) {
            health_points = 0;
            logger.log(character_name + " has been defeated!");
            throw std::runtime_error("Health dropped below zero for " + character_name);
        }
        logger.log(character_name + " takes " + std::to_string(damage) + " damage, HP now " + std::to_string(health_points));
    }

    void heal(int amount, Logger<std::string>& logger) {
        if (amount < 0) {
            throw std::invalid_argument("Heal amount must Vollständig-negative.");
        }
        health_points += amount;
        if (health_points > 100) health_points = 100;
        logger.log(character_name + " heals for " + std::to_string(amount) + " HP!");
    }

    void gainExperience(int exp, Logger<std::string>& logger) {
        if (exp < 0) {
            throw std::invalid_argument("Experience must be non-negative.");
        }
        exp_points += exp;
        logger.log(character_name + " gains " + std::to_string(exp) + " experience!");
        while (exp_points >= 100) {
            character_level++;
            exp_points -= 100;
            logger.log(character_name + " leveled up to level " + std::to_string(character_level) + "!");
        }
    }

    void addItem(const std::string& item, Logger<std::string>& logger) {
        inventory.addItem(item);
        logger.log(character_name + " added " + item + " to inventory.");
    }

    void removeItem(const std::string& item, Logger<std::string>& logger) {
        inventory.removeItem(item);
        logger.log(character_name + " removed " + item + " from inventory.");
    }

    void displayInfo() const {
        std::cout << "Name: " << character_name << ", HP: " << health_points
            << ", Attack: " << attack_power << ", Defense: " << defense_value
            << ", Level: " << character_level << ", Experience: " << exp_points << std::endl;
        inventory.displayInventory();
    }

    std::string serialize() const {
        return character_name + "," + std::to_string(health_points) + "," + std::to_string(attack_power) + "," +
            std::to_string(defense_value) + "," + std::to_string(character_level) + "," +
            std::to_string(exp_points) + "," + inventory.serialize();
    }

    void deserialize(const std::string& data, Logger<std::string>& logger) {
        std::istringstream iss(data);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }
        if (tokens.size() < 7) {
            throw std::runtime_error("Invalid character data format.");
        }
        character_name = tokens[0];
        try {
            health_points = std::stoi(tokens[1]);
            attack_power = std::stoi(tokens[2]);
            defense_value = std::stoi(tokens[3]);
            character_level = std::stoi(tokens[4]);
            exp_points = std::stoi(tokens[5]);
            inventory.deserialize(tokens[6]);
            validate();
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Invalid numeric data in character: " + std::string(e.what()));
        }
        logger.log("Loaded character: " + character_name);
    }

    std::string getName() const { return character_name; }
    int getHealth() const { return health_points; }
    int getAttack() const { return attack_power; }
    int getDefense() const { return defense_value; }
private:
    void validate() const {
        if (health_points <= 0) throw std::invalid_argument("Health must be positive.");
        if (character_name.empty()) throw std::invalid_argument("Name must not be empty.");
        if (attack_power < 0 || defense_value < 0) throw std::invalid_argument("Attack and defense must be non-negative.");
        if (character_level < 1) throw std::invalid_argument("Level must be at least 1.");
        if (exp_points < 0) throw std::invalid_argument("Experience must be non-negative.");
    }
};

// Game class
class Game {
private:
    Character player;
    std::vector<std::unique_ptr<Monster>> monsters;
    Logger<std::string> logger;
    bool running;

public:
    Game(const std::string& playerName)
        : player(playerName, 100, 45, 10), logger("game.log"), running(true) {
    }

    void addMonster(std::unique_ptr<Monster> monster) {
        monsters.push_back(std::move(monster));
        logger.log("Added monster: " + monsters.back()->getName());
    }

    bool isLichAlive() const {
        for (const auto& monster : monsters) {
            if (dynamic_cast<Lich*>(monster.get()) && monster->getHealth() > 0) {
                return true;
            }
        }
        return false;
    }

    void combat() {
        if (monsters.empty()) {
            std::cout << "No monsters to fight!\n";
            return;
        }

        // Display available monsters to attack
        std::cout << "\nChoose a monster to attack:\n";
        for (size_t i = 0; i < monsters.size(); ++i) {
            std::cout << (i + 1) << ". " << monsters[i]->getName()
                << " (HP: " << monsters[i]->getHealth() << ")\n";
        }
        std::cout << "Enter the number of the monster to attack: ";
        size_t target_index;
        if (!(std::cin >> target_index)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            return;
        }

        // Validate input
        if (target_index < 1 || target_index > monsters.size()) {
            std::cout << "Invalid monster selection.\n";
            return;
        }
        target_index--; // Convert to 0-based index

        auto& monster = *monsters[target_index];
        std::cout << "\nFighting " << monster.getName() << "!\n";

        bool monster_defeated = false;
        try {
            // Player attacks the selected monster
            player.attackEnemy(monster, logger);
            if (monster.getHealth() > 0) {
                // Monster retaliates if still alive
                int damage = monster.getAttack() - player.getDefense();
                if (damage < 0) damage = 0; // Clamp negative damage
                player.takeDamage(damage, logger);
            }
            else {
                monster_defeated = true;
            }
        }
        catch (const std::runtime_error& e) {
            std::cout << "Combat ended: " << e.what() << "\n";
            if (player.getHealth() <= 0) {
                running = false;
                return;
            }
            if (monster.getHealth() <= 0) {
                monster_defeated = true;
            }
        }

        // Handle monster defeat
        if (monster_defeated) {
            player.gainExperience(50, logger);
            if (auto* skeleton = dynamic_cast<Skeleton*>(&monster)) {
                if (isLichAlive()) {
                    // Resurrect skeleton immediately
                    monsters[target_index] = std::make_unique<Skeleton>(skeleton->getName(), 40, 10, 15);
                    logger.log(skeleton->getName() + " has been resurrected by the Lich!");
                }
                else {
                    monsters.erase(monsters.begin() + target_index);
                }
            }
            else {
                monsters.erase(monsters.begin() + target_index);
            }
        }
    }

    void saveProgress(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing: " + filename);
        }
        file << player.serialize() << "\n";
        file << monsters.size() << "\n";
        for (const auto& monster : monsters) {
            file << monster->serialize() << "\n";
        }
        logger.log("Game progress saved to " + filename);
    }

    void loadProgress(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for reading: " + filename);
        }

        std::string line;
        if (!std::getline(file, line)) {
            throw std::runtime_error("Failed to read player data from file.");
        }
        player.deserialize(line, logger);

        size_t monsterCount;
        if (!(file >> monsterCount)) {
            throw std::runtime_error("Failed to read monster count from file.");
        }
        std::getline(file, line); // Consume newline
        monsters.clear();
        for (size_t i = 0; i < monsterCount; ++i) {
            if (!std::getline(file, line)) {
                throw std::runtime_error("Failed to read monster data from file.");
            }
            std::istringstream iss(line);
            std::string type, name;
            int health, attack, defense;
            char comma;
            if (!std::getline(iss, type, ',') || !std::getline(iss, name, ',') ||
                !(iss >> health >> comma >> attack >> comma >> defense)) {
                throw std::runtime_error("Invalid monster data format: " + line);
            }
            try {
                if (type == "Skeleton") {
                    monsters.push_back(std::make_unique<Skeleton>(name, health, attack, defense));
                }
                else if (type == "Lich") {
                    monsters.push_back(std::make_unique<Lich>(name, health, attack, defense));
                }
                else {
                    throw std::runtime_error("Unknown monster type: " + type);
                }
                logger.log("Loaded monster: " + name);
            }
            catch (const std::exception& e) {
                std::cerr << "Warning: Failed to load monster: " << e.what() << "\n";
            }
        }
        logger.log("Game progress loaded from " + filename);
    }

    void play() {
        while (running) {
            std::cout << "\nPlayer Info:\n";
            player.displayInfo();
            std::cout << "\nMonsters:\n";
            for (size_t i = 0; i < monsters.size(); ++i) {
                std::cout << (i + 1) << ". ";
                monsters[i]->displayInfo();
            }
            std::cout << "\nOptions: (1) Fight, (2) Heal, (3) Add Item, (4) Remove Item, (5) Save, (6) Load, (7) Exit\n";
            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number.\n";
                continue;
            }
            try {
                switch (choice) {
                case 1:
                    combat();
                    break;
                case 2:
                    player.heal(20, logger);
                    break;
                case 3: {
                    std::string item;
                    std::cout << "Enter item name: ";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, item);
                    player.addItem(item, logger);
                    break;
                }
                case 4: {
                    std::string item;
                    std::cout << "Enter item name: ";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, item);
                    player.removeItem(item, logger);
                    break;
                }
                case 5:
                    saveProgress("game_save.txt");
                    break;
                case 6:
                    loadProgress("game_save.txt");
                    break;
                case 7:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid option. Please choose between 1 and 7.\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        }
        logger.log("Game ended.");
    }
};

// main
int main() {
    try {
        Game game("Hero");
        game.addMonster(std::make_unique<Skeleton>("Skeleton1"));
        game.addMonster(std::make_unique<Skeleton>("Skeleton2"));
        game.addMonster(std::make_unique<Lich>("LichKing"));
        game.play();
    }
    catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}