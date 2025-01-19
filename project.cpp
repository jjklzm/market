#include <iostream> // Для работы с вводом и выводом
#include <vector>   // Для работы с динамическими массивами
#include <string>   // Для работы с типом данных string
#include <thread>   // Для использования многозадачности и задержек
#include <chrono>   // Для работы с временем
#include <cstdlib>  // Для использования функции rand()

// Простая структура товара
struct Product {
    std::string name;  // Название товара
    double price;      // Цена товара
    std::string recommended; // Рекомендуемый товар, если есть
};

// Функция для очистки экрана (симуляция в консоли)
void clearScreen() {
    std::cout << std::string(100, '\n'); // Выводит 100 пустых строк для симуляции очистки экрана
}

// Функция для отображения списка товаров
void displayProducts(const std::vector<Product>& products) {
    for (size_t i = 0; i < products.size(); ++i) { // Перебираем все товары в векторе
        std::cout << i + 1 << ". " << products[i].name << " - $" << products[i].price << "\n"; // Выводим название и цену товара
    }
}

// Функция для покупки товара
void buyProduct(std::vector<Product>& inventory, const std::vector<Product>& products, int index, double& cash, std::vector<Product>& recommended) {
    if (index >= 0 && index < static_cast<int>(products.size())) { // Проверяем, что индекс корректен
        if (cash >= products[index].price) { // Если у пользователя достаточно денег
            inventory.push_back(products[index]); // Добавляем товар в инвентарь
            cash -= products[index].price; // Вычитаем цену товара из баланса
            std::cout << "You bought: " << products[index].name << "\n"; // Сообщаем пользователю о покупке

            // Добавляем товар в рекомендуемые
            if (!products[index].recommended.empty()) { // Если есть рекомендуемый товар
                recommended.push_back({ products[index].recommended, products[index].price * 0.9, "" }); // Добавляем рекомендуемый товар с уменьшенной ценой
            }
        }
        else {
            std::cout << "Not enough cash!\n"; // Если денег недостаточно
        }
    }
    else {
        std::cout << "Invalid choice.\n"; // Если выбор некорректен
    }
}

// Основная программа
int main() {
    // Товары в маркете
    std::vector<Product> market = {
        {"Fork", 1.5, "Spoon"}, // Вилка, цена 1.5$, рекомендуемый товар - ложка
        {"Plate", 3.0, "Glass"}, // Тарелка, цена 3.0$, рекомендуемый товар - стакан
        {"Cup", 2.0, "Knife"}, // Чашка, цена 2.0$, рекомендуемый товар - нож
        {"Pistol", 200.0, "Revolver"}, // Пистолет, цена 200.0$, рекомендуемый товар - револьвер
        {"Revolver", 300.0, "Ammo"}, // Револьвер, цена 300.0$, рекомендуемый товар - патроны
        {"Knife", 25.0, "Sharpening Stone"}, // Нож, цена 25.0$, рекомендуемый товар - точилка
        {"Rifle", 500.0, "Scope"}, // Винтовка, цена 500.0$, рекомендуемый товар - прицел
        {"Ammo", 50.0, "Rifle"}, // Патроны, цена 50.0$, рекомендуемый товар - винтовка
        {"Scope", 100.0, "Sniper Rifle"}, // Прицел, цена 100.0$, рекомендуемый товар - снайперская винтовка
        {"Sniper Rifle", 800.0, "Ammo"}, // Снайперская винтовка, цена 800.0$, рекомендуемый товар - патроны
        {"Helmet", 75.0, "Armor"}, // Шлем, цена 75.0$, рекомендуемый товар - броня
        {"Armor", 150.0, "Shield"}, // Броня, цена 150.0$, рекомендуемый товар - щит
        {"Shield", 200.0, "Sword"}, // Щит, цена 200.0$, рекомендуемый товар - меч
        {"Sword", 250.0, "Shield"}, // Меч, цена 250.0$, рекомендуемый товар - щит
        {"Sharpening Stone", 10.0, "Knife"}, // Точилка, цена 10.0$, рекомендуемый товар - нож
    };

    // Товары на рынке
    std::vector<Product> marketplace = {
        {"Spoon", 1.0, ""}, // Ложка, цена 1.0$, без рекомендаций
        {"Knife", 2.5, ""}, // Нож, цена 2.5$, без рекомендаций
        {"Glass", 2.8, ""}, // Стакан, цена 2.8$, без рекомендаций
        {"Lantern", 15.0, ""}, // Фонарь, цена 15.0$, без рекомендаций
        {"Backpack", 50.0, ""}, // Рюкзак, цена 50.0$, без рекомендаций
    };

    // Инвентарь пользователя
    std::vector<Product> inventory; // Пустой инвентарь

    // Рекомендуемые товары
    std::vector<Product> recommended; // Пустой список рекомендуемых товаров

    // Баланс пользователя
    double cash = 100.0; // Начальный баланс 100$

    // Основной цикл программы
    bool running = true; // Переменная для продолжения работы программы
    while (running) {
        clearScreen(); // Очищаем экран
        std::cout << "\n=== Internet Shop Menu ===\n"; // Выводим меню
        std::cout << "Your Cash: $" << cash << "\n"; // Показываем баланс пользователя
        std::cout << "1. Market\n"; // Опция для перехода в маркет
        std::cout << "2. Recommended\n"; // Опция для просмотра рекомендуемых товаров
        std::cout << "3. Marketplace\n"; // Опция для перехода на рынок
        std::cout << "4. Inventory\n"; // Опция для просмотра инвентаря
        std::cout << "5. Borrow Money\n"; // Опция для взятия кредита
        std::cout << "6. Exit\n"; // Опция для выхода
        std::cout << "Choose an option: "; // Запрос выбора

        int choice;
        std::cin >> choice; // Считываем выбор пользователя

        switch (choice) {
        case 1: { // Маркет
            clearScreen(); // Очищаем экран
            std::cout << "\nMarket Items:\n"; // Выводим список товаров на маркет
            displayProducts(market); // Вызываем функцию для отображения товаров в маркете
            std::cout << "\nEnter the number of the product to buy (or 0 to go back): "; // Запрос выбора товара для покупки
            int buyChoice;
            std::cin >> buyChoice; // Считываем выбор пользователя

            if (buyChoice > 0 && buyChoice <= static_cast<int>(market.size())) { // Если выбор корректен
                buyProduct(inventory, market, buyChoice - 1, cash, recommended); // Вызываем функцию для покупки товара
            }
            std::cout << "Press Enter to continue..."; // Просим нажать Enter для продолжения
            std::cin.ignore(); // Игнорируем предыдущий символ новой строки
            std::cin.get(); // Ожидаем нажатия Enter
            break;
        }
        case 2: { // Рекомендуемые
            clearScreen(); // Очищаем экран
            std::cout << "\nRecommended Items:\n"; // Выводим список рекомендуемых товаров
            if (recommended.empty()) { // Если рекомендуемых товаров нет
                std::cout << "No recommendations yet. Buy something from the Market!\n"; // Сообщаем, что нет рекомендаций
            }
            else {
                displayProducts(recommended); // Выводим список рекомендуемых товаров
            }
            std::cout << "Press Enter to continue..."; // Просим нажать Enter для продолжения
            std::cin.ignore(); // Игнорируем символ новой строки
            std::cin.get(); // Ожидаем нажатия Enter
            break;
        }
        case 3: { // Рынок
            clearScreen(); // Очищаем экран
            std::cout << "\nMarketplace Items:\n"; // Выводим список товаров на рынке
            displayProducts(marketplace); // Выводим товары с текущими ценами
            std::cout << "Updating in 10 seconds...\n"; // Информируем, что обновление произойдет через 10 секунд
            std::this_thread::sleep_for(std::chrono::seconds(10)); // Задержка в 10 секунд для симуляции обновления рынка

            // Обновляем рынок (рандомно меняем цены)
            for (auto& product : marketplace) { // Перебираем все товары на рынке
                product.price = (rand() % 1000) / 100.0; // Генерируем новую цену от 0 до 10$
            }
            break;
        }
        case 4: { // Инвентарь
            clearScreen(); // Очищаем экран
            std::cout << "\nYour Inventory:\n"; // Выводим инвентарь пользователя
            if (inventory.empty()) { // Если инвентарь пуст
                std::cout << "Your inventory is empty. Buy something!\n"; // Сообщаем, что инвентарь пуст
            }
            else {
                displayProducts(inventory); // Выводим товары в инвентаре
            }
            std::cout << "Press Enter to continue..."; // Просим нажать Enter для продолжения
            std::cin.ignore(); // Игнорируем символ новой строки
            std::cin.get(); // Ожидаем нажатия Enter
            break;
        }
        case 5: { // Занять деньги
            cash += 50.0; // Увеличиваем баланс на 50$
            std::cout << "\nYou borrowed $50. Your new balance: $" << cash << "\n"; // Сообщаем, что деньги были заняты
            std::cout << "Press Enter to continue..."; // Просим нажать Enter для продолжения
            std::cin.ignore(); // Игнорируем символ новой строки
            std::cin.get(); // Ожидаем нажатия Enter
            break;
        }
        case 6: { // Выход
            running = false; // Завершаем выполнение программы
            std::cout << "Goodbye!\n"; // Сообщаем об окончании работы программы
            break;
        }
        default: { // Некорректный выбор
            std::cout << "Invalid choice. Please try again.\n"; // Сообщаем, что выбор некорректен
            break;
        }
        }
    }

    return 0; // Завершаем программу
}
