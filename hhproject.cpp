#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <filesystem> 
#include <ctime>

struct Product {
    std::string name;
    double price;
    std::string recommended;
};

struct Debt {
    double amount;
    int turnsLeft;
};


void clearScreen() {
    std::cout << std::string(100, '\n');
}

void displayProducts(const std::vector<Product>& products) {
    for (size_t i = 0; i < products.size(); ++i) {
        std::cout << i + 1 << ". " << products[i].name << " - $" << products[i].price << "\n";
    }
}

void displayInventory(const std::vector<Product>& inventory) {
    std::cout << "Your inventory has these items:\n";
    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << i + 1 << ". " << inventory[i].name << " - $" << inventory[i].price << "\n";
    }
}

void buyProduct(std::vector<Product>& inventory, const std::vector<Product>& products, int index, double& cash, std::vector<Product>& recommended, int& turnsPassed) {
    if (index >= 0 && index < static_cast<int>(products.size())) {
        if (cash >= products[index].price) {
            inventory.push_back(products[index]);
            cash -= products[index].price;
            std::cout << "You bought: " << products[index].name << "\n";

            if (!products[index].recommended.empty()) {
                recommended.push_back({ products[index].recommended, products[index].price * 0.9, "" });
            }
            turnsPassed++;
        }
        else {
            std::cout << "Not enough money!\n";
        }
    }
    else {
        std::cout << "Wrong choice.\n";
    }
}

void sellProduct(std::vector<Product>& inventory, double& cash, int index, int& turnsPassed) {
    if (index >= 0 && index < static_cast<int>(inventory.size())) {
        cash += inventory[index].price;
        std::cout << "You sold: " << inventory[index].name << "\n";
        inventory.erase(inventory.begin() + index);
        turnsPassed++;
    }
    else {
        std::cout << "Wrong choice to sell.\n";
    }
}

void borrowMoney(double& cash, std::vector<Debt>& debts, int& turnsPassed) {
    double amount;
    std::cout << "Enter the amount you want to borrow: ";
    std::cin >> amount;

    if (amount > 0) {
        debts.push_back({ amount, 5 });
        cash += amount;
        std::cout << "You borrowed $" << amount << "\n";
        turnsPassed++;
    }
    else {
        std::cout << "You can't borrow a negative amount!\n";
    }
}

void saveInventoryToFile(const std::vector<Product>& inventory) {
    try {
        std::filesystem::create_directory("data");
        std::ofstream outFile("data/inventory.txt");
        if (!outFile) {
            throw std::ios_base::failure("Error opening a file");
        }

        for (const auto& item : inventory) {
            outFile << item.name << "," << item.price << "\n";
        }
        outFile.close();
        std::cout << "Inventory saved to data/inventory.txt\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving an inventory: " << e.what() << "\n";
    }
}

void payDebt(double& cash, std::vector<Debt>& debts, int& turnsPassed) {
    if (debts.empty()) {
        std::cout << "No debts to pay.\n";
        return;
    }

    std::cout << "Your debts:\n";
    for (size_t i = 0; i < debts.size(); ++i) {
        std::cout << i + 1 << ". Debt: $" << debts[i].amount << " (Turns Left: " << debts[i].turnsLeft << ")\n";
    }

    std::cout << "Choose a debt to pay off or enter 0 to cancel: ";
    int choice;
    std::cin >> choice;

    if (choice > 0 && choice <= static_cast<int>(debts.size())) {
        if (cash >= debts[choice - 1].amount) {
            cash -= debts[choice - 1].amount;
            debts.erase(debts.begin() + choice - 1);
            std::cout << "Debt paid off!\n";
            turnsPassed++;
        }
        else {
            std::cout << "Not enough cash to pay off this debt.\n";
        }
    }
    else {
        std::cout << "Wrong choice.\n";
    }
}

void checkDebtProgress(std::vector<Debt>& debts, double& cash, int& turnsPassed) {
    for (size_t i = 0; i < debts.size(); ++i) {
        debts[i].turnsLeft--;
        if (debts[i].turnsLeft <= 0) {
            std::cout << "You failed to pay off your debt of $" << debts[i].amount << " in time.\n";
            double penalty = debts[i].amount * 0.30;
            cash -= (debts[i].amount + penalty);
            std::cout << "A penalty of $" << penalty << " has been applied.\n";
            debts.erase(debts.begin() + i);
            i--;
        }
    }
}

void displayTurnStatus(int turnsPassed) {
    std::cout << "Turns passed: " << turnsPassed << "\n";
}


int main() {
    std::vector<Product> market = {
        {"Fork", 1.5, "Spoon"},
        {"Plate", 3.0, "Glass"},
        {"Cup", 2.0, "Knife"},
        {"Pistol", 200.0, "Revolver"},
        {"Revolver", 300.0, "Ammo"},
        {"Knife", 25.0, "Sharpening Stone"},
        {"Rifle", 500.0, "Scope"},
        {"Ammo", 50.0, "Rifle"},
        {"Scope", 100.0, "Sniper Rifle"},
        {"Sniper Rifle", 800.0, "Ammo"},
        {"Helmet", 75.0, "Armor"},
        {"Armor", 150.0, "Shield"},
        {"Shield", 200.0, "Sword"},
        {"Sword", 250.0, "Shield"},
        {"Sharp Stone", 10.0, "Knife"},
    };

    std::vector<Product> marketplace = {
        {"Spoon", 1.0, ""},
        {"Knife", 2.5, ""},
        {"Glass", 2.8, ""},
        {"Lantern", 15.0, ""},
        {"Backpack", 50.0, ""},
    };

    std::vector<Product> inventory;
    std::vector<Product> recommended;
    std::vector<Debt> debts;
    double cash = 100.0;
    int debtTurns = 5;
    int turnsPassed = 0;

    bool running = true;
    while (running) {
        clearScreen();
        std::cout << "\n=== Internet Shop Menu ===\n";
        std::cout << "Your Cash: $" << cash << "\n";
        displayTurnStatus(turnsPassed);
        std::cout << "1. Market\n";
        std::cout << "2. Recommended\n";
        std::cout << "3. Marketplace\n";
        std::cout << "4. Inventory\n";
        std::cout << "5. Sell Item\n";
        std::cout << "6. Save Inventory\n";
        std::cout << "7. Pay Debt\n";
        std::cout << "8. Borrow Money\n";
        std::cout << "9. Exit\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            clearScreen();
            std::cout << "\nMarket Items:\n";
            displayProducts(market);
            std::cout << "\nEnter the number of the product to buy (or 0 to go back): ";
            int buyChoice;
            std::cin >> buyChoice;

            if (buyChoice > 0 && buyChoice <= static_cast<int>(market.size())) {
                buyProduct(inventory, market, buyChoice - 1, cash, recommended, turnsPassed);
            }
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        }
        case 2: {
            clearScreen();
            std::cout << "\nRecommended Items:\n";
            if (recommended.empty()) {
                std::cout << "No recommendations yet. Buy something from the Market!\n";
            }
            else {
                displayProducts(recommended);
            }
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        }
        case 3: {
            clearScreen();
            std::cout << "\nMarketplace Items:\n";
            displayProducts(marketplace);
            std::cout << "\nEnter the number of the product to buy (or 0 to refresh prices): ";
            int marketChoice;
            std::cin >> marketChoice;

            if (marketChoice > 0 && marketChoice <= static_cast<int>(marketplace.size())) {
                buyProduct(inventory, marketplace, marketChoice - 1, cash, recommended, turnsPassed);
            }
            else {
                for (auto& product : marketplace) {
                    product.price = (rand() % 1000) / 100.0;
                }
                std::cout << "Prices updated!\n";
            }
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        }
        case 4: {
            clearScreen();
            std::cout << "\nYour Inventory:\n";
            if (inventory.empty()) {
                std::cout << "Your inventory is empty. Buy something!\n";
            }
            else {
                displayInventory(inventory);
            }
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        }
        case 5: {
            clearScreen();
            std::cout << "\nYour Inventory:\n";
            displayInventory(inventory);
            std::cout << "\nEnter the number of the item to sell (or 0 to go back): ";
            int sellChoice;
            std::cin >> sellChoice;

            if (sellChoice > 0 && sellChoice <= static_cast<int>(inventory.size())) {
                sellProduct(inventory, cash, sellChoice - 1, turnsPassed);
            }
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        }
        case 6: {
            saveInventoryToFile(inventory);
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        }
        case 7: {
            payDebt(cash, debts, turnsPassed);
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        }
        case 8: {
            borrowMoney(cash, debts, turnsPassed);
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            std::cin.get();
            break;
        }
        case 9: {
            running = false;
            std::cout << "Goodbye!\n";
            break;
        }
        default: {
            std::cout << "Wrong choice. Please try again.\n";
            break;
        }
        }

        checkDebtProgress(debts, cash, turnsPassed);
    }

    return 0;
}
