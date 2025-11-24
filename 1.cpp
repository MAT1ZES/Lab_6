#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

struct Driver
{
    string name;
    string car;
    double rating;
};

struct User
{
    string name, login, password;
};

struct Order
{
    string from, to, driver, car;
    double price = 0.00;
    double distance = 0;
    int travelTime = 0;
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

const int NUM_DRIVERS = 10;


// робимо логін нечутливим до регістру
string toLower(string s)
{
    for (char& c : s) c = tolower(c);
    return s;
}

// реєстрація + перевірка порожніх полів (fix RegistrationEmptyFieldsTest)
User registration()
{
    User u;

    cout << "=== Реєстрація ===\n";

    do {
        cout << "Ім'я: ";
        getline(cin, u.name);
        if (u.name.empty()) cout << "Поля не можуть бути порожніми!\n";
    } while (u.name.empty());

    do {
        cout << "Логін: ";
        getline(cin, u.login);
        if (u.login.empty()) cout << "Поля не можуть бути порожніми!\n";
    } while (u.login.empty());

    do {
        cout << "Пароль: ";
        getline(cin, u.password);
        if (u.password.empty()) cout << "Поля не можуть бути порожніми!\n";
    } while (u.password.empty());

    cout << "\nРеєстрація успішна!\n\n";

    // зберігаємо логін у нижньому регістрі
    u.login = toLower(u.login);

    return u;
}

// авторизація (fix LoginCaseSensitiveTest)
bool login(const User& u)
{
    string l, p;

    cout << "=== Вхід ===\n";
    cout << "Логін: ";
    getline(cin, l);
    cout << "Пароль: ";
    getline(cin, p);

    if (toLower(l) == u.login && p == u.password)
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

    // адреса ЗВІДКИ
    do {
        cout << "Адреса звідки: ";
        getline(cin, o.from);

        if (!isValidAddress(o.from))
            cout << "Помилка! Невірна адреса.\n";

    } while (!isValidAddress(o.from));

    // адреса КУДИ
    do {
        cout << "Адреса куди: ";
        getline(cin, o.to);

        if (!isValidAddress(o.to))
        {
            cout << "Помилка! Невірна адреса.\n";
            o.to = "";
        }
        else if (o.to == o.from)
        {
            cout << "Адреси не можуть збігатися!\n";
            o.to = "";
        }

    } while (o.to.empty());

    // рандомні водії
    int availableCount = 1 + rand() % NUM_DRIVERS;

    for (int i = 0; i < NUM_DRIVERS - 1; ++i)
    {
        int j = i + rand() % (NUM_DRIVERS - i);
        swap(availableDrivers[i], availableDrivers[j]);
    }

    cout << "\nДоступні Водії ( " << availableCount << " )\n";
    cout << left << setw(3) << "№" << setw(15) << "Водій" << setw(30)
        << "Авто" << "Рейтинг\n";

    for (int i = 0; i < availableCount; i++)
    {
        cout << left << setw(3) << i + 1
            << setw(15) << availableDrivers[i].name
            << setw(30) << availableDrivers[i].car
            << availableDrivers[i].rating << "\n";
    }

    while (!validChoice)
    {
        cout << "Оберіть номер водія: ";
        if (cin >> choice && choice >= 1 && choice <= availableCount)
        {
            o.driver = availableDrivers[choice - 1].name;
            o.car = availableDrivers[choice - 1].car;
            validChoice = true;
        }
        else
        {
            cout << "Невірний вибір!\n";
            cin.clear();
        }
        cin.ignore(1000, '\n');
    }

    // авто генерація параметрів
    o.distance = 2 + rand() % 10;
    o.travelTime = 5 + rand() % 15;

    o.price = o.distance * 25 + o.travelTime * 1.5;

    cout << "\nВаше замовлення підтверджено!\n";

    return o;
}

// --- payment() з тестами (валідність картки + баланс) ---
void payment(double price)
{
    int method;

    cout << "\n=== Оплата ===\n";
    cout << "Сума: " << price << " грн\n";
    cout << "1 - Картка\n2 - Готівка\n";

    while (!(cin >> method) || (method != 1 && method != 2))
    {
        cout << "Помилка! Введіть 1 або 2: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    // Картка
    if (method == 1)
    {
        string card;
        cout << "Номер картки (16 цифр): ";
        cin >> card;

        bool ok = (card.length() == 16);
        for (char c : card)
            if (!isdigit(c)) ok = false;

        if (!ok)
        {
            cout << "Некоректний номер картки!\n";
            return;
        }

        double balance;
        cout << "Баланс картки: ";
        cin >> balance;

        if (balance < price)
        {
            cout << "Недостатньо коштів!\n";
            return;
        }

        cout << "Оплата карткою успішна.\n";
    }
    else
    {
        double cash;
        cout << "Готівка: ";
        cin >> cash;

        if (cash < price)
        {
            cout << "Недостатньо коштів!\n";
            return;
        }

        cout << "Решта: " << cash - price << " грн\n";
    }

    cin.ignore(1000, '\n');
}

// оцінка
void rating()
{
    int score;
    cout << "\nОцінка (1–5): ";

    while (!(cin >> score) || score < 1 || score > 5)
    {
        cout << "Помилка! Введіть 1–5: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    cin.ignore(1000, '\n');

    string comment;
    cout << "Коментар: ";
    getline(cin, comment);

    cout << "Дякуємо за відгук!\n";
}

int main()
{
    setupUkr();

    cout << "=== ДОДАТОК ТАКСІ ===\n";

    User user = registration();
    if (!login(user)) return 0;

    Order order = makeOrder();
    payment(order.price);
    rating();

    cout << "\nДякуємо, " << user.name << "!\n";
}
