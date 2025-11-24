#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>   // для rand() і srand()
#include <ctime>     // для time()
#include <sstream>   
#include <algorithm> // Для std::swap
using namespace std;

struct Driver
{
    string name;
    string car;
    double rating; // Рейтинг
};

struct User
{
    string name, login, password;
};

struct Order
{
    string from, to, driver, car;
    double price = 0.00;
    double distance = 0;  // км
    int travelTime = 0;   // хв
};


Driver availableDrivers[10] =
{
    {"Петро", "Renault Logan (KA2668LI)", 4.7},
    {"Олена", "Toyota Camry (AA1234EE)", 5.0},
    {"Сергій", "Skoda Octavia (BC5678BC)", 4.5},
    {"Анна", "Hyundai Elantra (CC9012CD)", 4.9},
    {"Дмитро", "Lada Granta (DE3456EF)", 4.1},
    {"Марина", "Kia Rio (FG7890GH)", 4.7},
    {"Іван", "Volkswagen Passat (HI1122IJ)", 4.6},
    {"Катерина", "Ford Focus (JK3344KL)", 4.9},
    {"Олексій", "Chevrolet Aveo (LM5566MN)", 4.3},
    {"Тетяна", "Nissan Qashqai (NO7788OP)", 5.0}
};

const int NUM_DRIVERS = 10; // rонстанта для розміру масиву


// реєстрація користувача
User registration()
{
    User u;
    cout << "=== Реєстрація ===\n";
    cout << "Ім'я: "; getline(cin, u.name);
    cout << "Логін: "; getline(cin, u.login);
    cout << "Пароль: "; getline(cin, u.password);
    cout << "\nРеєстрація успішна!\n\n";
    return u;
}

// авторизація
bool login(const User& u)
{
    string l, p;
    cout << "=== Вхід ===\n";
    cout << "Логін: "; getline(cin, l);
    cout << "Пароль: "; getline(cin, p);
    if (l == u.login && p == u.password)
    {
        cout << "\nАвторизація успішна!\n";
        return true;
    }
    cout << "Помилка входу!\n";
    return false;
}

// оформлення замовлення
// Допоміжна функція для перевірки коректності адреси
// Виправляє OrderSpecialSymbolsTest та OrderEmptyInputTest
bool isValidAddress(const string& address) {
    // 1. Перевірка на порожність
    if (address.empty()) return false;

    bool hasLettersOrDigits = false;
    
    for (char c : address) {
        // 2. Перевірка на заборонені спецсимволи (як у тесті з '@@@')
        // Можна розширити список, якщо треба
        if (c == '@' || c == '#' || c == '$' || c == '%') {
            return false; 
        }
        
        // 3. Перевірка, чи є хоча б одна літера або цифра 
        // (щоб користувач не ввів просто пробіли або крапки)
        if (isalnum(c)) { 
            hasLettersOrDigits = true;
        }
    }
    
    // Адреса валідна тільки якщо немає заборонених знаків І є хоча б одна буква/цифра
    return hasLettersOrDigits;
}

Order makeOrder()
{
    Order o;
    int choice;
    bool validChoice = false;

    cout << "\n Оформлення замовлення \n";

    // --- БЛОК 1: Введення адреси "Звідки" ---
    do {
        cout << "Адреса звідки: ";
        getline(cin, o.from);

        if (!isValidAddress(o.from)) {
            cout << "Помилка! Адреса не може бути порожньою або містити спецсимволи (@, #, $). Спробуйте ще раз.\n";
        }
    } while (!isValidAddress(o.from)); // Повторюємо, доки адреса не стане коректною


    // --- БЛОК 2: Введення адреси "Куди" ---
    do {
        cout << "Адреса куди: ";
        getline(cin, o.to);

        // Спочатку перевіряємо формат адреси
        if (!isValidAddress(o.to)) {
             cout << "Помилка! Адреса не може бути порожньою або містити спецсимволи (@, #, $).\n";
             o.to = ""; // Скидаємо значення, щоб цикл продовжився
        } 
        // ВИПРАВЛЕННЯ ПОМИЛКИ OrderSameAddressesTest
        else if (o.from == o.to) {
            cout << "Помилка! Адреса відправлення та призначення не можуть збігатися. Введіть іншу адресу.\n";
            o.to = ""; // Скидаємо значення, щоб цикл продовжився
        }

    } while (o.to.empty()); // Цикл працює, доки не отримаємо валідну, унікальну адресу


    // --- Далі йде стандартна логіка програми (без змін) ---

    // Рандомно визначаємо кількість доступних водіїв (від 1 до 10)
    int availableCount = 1 + rand() % NUM_DRIVERS;

    // Перемішуємо масив водіїв
    for (int i = 0; i < NUM_DRIVERS - 1; ++i)
    {
        int j = i + rand() % (NUM_DRIVERS - i);
        swap(availableDrivers[i], availableDrivers[j]);
    }

    cout << "\nДоступні Водії та Авто (Кількість: " << availableCount << ")\n";
    cout << left << setw(3) << "№" << setw(15) << "Водій" << setw(30) << "Авто" << "Рейтинг\n";

    for (int i = 0; i < availableCount; ++i)
    {
        cout << left << setw(3) << i + 1
             << setw(15) << availableDrivers[i].name
             << setw(30) << availableDrivers[i].car
             << fixed << setprecision(1) << availableDrivers[i].rating << "\n";
    }

    while (!validChoice)
    {
        cout << "Оберіть номер водія (1-" << availableCount << "): ";
        if (cin >> choice)
        {
            if (choice >= 1 && choice <= availableCount)
            {
                o.driver = availableDrivers[choice - 1].name;
                o.car = availableDrivers[choice - 1].car;
                validChoice = true;
            }
            else
            {
                cout << "Невірний номер. Введіть число від 1 до " << availableCount << ".\n";
            }
        }
        else 
        {
            cout << "Некоректне введення. Спробуйте ще раз.\n";
            cin.clear();
            string dummy;
            getline(cin, dummy);
        }
    }
    cin.ignore(); 

    o.distance = 2 + rand() % 10;          
    o.travelTime = 5 + rand() % 15;        

    double costPerKm = 25.0;
    double costPerMin = 1.5;
    o.price = (o.distance * costPerKm) + (o.travelTime * costPerMin);

    cout << "\nВаше замовлення підтверджено!\n";
    cout << "Водій: " << o.driver << "\n";
    cout << "Авто: " << o.car << "\n";
    cout << "Відстань: " << o.distance << " км\n";
    cout << "Час у дорозі: " << o.travelTime << " хв\n";
    cout << fixed << setprecision(2);
    cout << "Орієнтовна вартість: " << o.price << " грн\n";
    
    return o;
}

void payment(double price)
{
    int method;
    cout << "\n Оплата \n";
    cout << "Сума до сплати: " << fixed << setprecision(2) << price << " грн\n";
    cout << "Оберіть спосіб оплати:\n1 - Картка\n2 - Готівка\nВаш вибір: ";

    while (!(cin >> method) || (method != 1 && method != 2))
    {
        cout << "Невірний вибір! Оберіть 1 або 2: ";
        cin.clear();
        string dummy;
        getline(cin, dummy);
    }

    if (method == 1)
    {
        cout << "Оплата карткою успішна.\n";
    }
    else if (method == 2)
    {
        double cash;
        cout << "Введіть суму, яку ви даєте: ";

        while (!(cin >> cash) || cash <= 0)
        {
            cout << "Некоректна сума. Введіть додатне число: ";
            cin.clear();
            string dummy;
            getline(cin, dummy);
        }

        if (cash < price)
        {
            cout << "Недостатньо коштів! Оплата не виконана.\n";
            cin.ignore();
            return;
        }

        double change = cash - price;
        cout << "Оплата готівкою прийнята.\n";
        cout << "Ваша решта: " << fixed << setprecision(2) << change << " грн\n";
    }

    cin.ignore();
}


// оцінка поїздки
void rating()
{
    int score;
    string comment;
    cout << "\n Оцінка поїздки \n";
    cout << "Оцінка (1–5): ";

    while (!(cin >> score) || score < 1 || score > 5)
    {
        cout << "Невірний діапазон. Введіть оцінку від 1 до 5: ";
        cin.clear();
        string dummy;
        getline(cin, dummy);
    }

    cin.ignore();
    cout << "Коментар: "; getline(cin, comment);
    cout << "Дякуємо за ваш відгук!\n";
}

int main()
{

    cout << " ДОДАТОК ТАКСІ\n";

    User user = registration();
    if (!login(user)) return 0;

    Order order = makeOrder();
    payment(order.price);
    rating();

    cout << "\nДякуємо, " << user.name << ", що скористалися нашим сервісом!\n";
    return 0;
}