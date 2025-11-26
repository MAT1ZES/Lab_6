#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

// Структура з даними про водія
struct Driver
{
    string name;
    string car;
    double rating;
};

// Структура користувача (логін, пароль)
struct User
{
    string name, login, password;
};

// Структура замовлення
struct Order
{
    string from, to, driver, car;
    double price = 0.00;
    double distance = 0;
    int travelTime = 0;
};

// Масив доступних водіїв
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


// Робимо логін нечутливим до регістру
string toLower(string s)
{
    for (char& c : s) c = tolower(c);
    return s;
}


// --- Реєстрація користувача ---
// Додаємо перевірку на порожні поля (тест RegistrationEmptyFieldsTest)
User registration()
{
    User u;

    cout << "=== Реєстрація ===\n";

    // Імʼя
    do {
        cout << "Ім'я: ";
        getline(cin, u.name);
        if (u.name.empty()) cout << "Поля не можуть бути порожніми!\n";
    } while (u.name.empty());

    // Логін
    do {
        cout << "Логін: ";
        getline(cin, u.login);
        if (u.login.empty()) cout << "Поля не можуть бути порожніми!\n";
    } while (u.login.empty());

    // Пароль
    do {
        cout << "Пароль: ";
        getline(cin, u.password);
        if (u.password.empty()) cout << "Поля не можуть бути порожніми!\n";
    } while (u.password.empty());

    cout << "\nРеєстрація успішна!\n\n";

    // Зберігаємо логін у нижньому регістрі (для тесту LoginCaseSensitiveTest)
    u.login = toLower(u.login);

    return u;
}


// --- Авторизація ---
// Порівнюємо логіни без врахування регістру
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


// --- Перевірка адреси (виправляє тести OrderSpecialSymbolsTest та OrderEmptyInputTest) ---
bool isValidAddress(const string& address)
{
    if (address.empty()) return false;

    bool hasLettersOrDigits = false;

    for (char c : address)
    {
        // Заборонені символи (@ @ # $ %)
        if (c == '@' || c == '#' || c == '$' || c == '%')
            return false;

        // Має бути хоч 1 буква або цифра
        if (isalnum(c))
            hasLettersOrDigits = true;
    }
    return hasLettersOrDigits;
}


// --- Оформлення замовлення ---
Order makeOrder()
{
    Order o;
    int choice;
    bool validChoice = false;

    cout << "\n Оформлення замовлення \n";

    // Введення адреси "звідки"
    do {
        cout << "Адреса звідки: ";
        getline(cin, o.from);

        if (!isValidAddress(o.from))
            cout << "Помилка! Невірна адреса.\n";

    } while (!isValidAddress(o.from));

    // Введення адреси "куди"
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

    // Перемішуємо список водіїв
    int availableCount = 1 + rand() % NUM_DRIVERS;

    for (int i = 0; i < NUM_DRIVERS - 1; ++i)
    {
        int j = i + rand() % (NUM_DRIVERS - i);
        swap(availableDrivers[i], availableDrivers[j]);
    }

    // Виводимо доступних водіїв
    cout << "\nДоступні Водії ( " << availableCount << " )\n";
    cout << left << setw(3) << "№" << setw(15) << "Водій"
        << setw(30) << "Авто" << "Рейтинг\n";

    for (int i = 0; i < availableCount; i++)
    {
        cout << left << setw(3) << i + 1
            << setw(15) << availableDrivers[i].name
            << setw(30) << availableDrivers[i].car
            << availableDrivers[i].rating << "\n";
    }

    // Вибір водія
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

    // Генерація відстані, часу та ціни
    o.distance = 2 + rand() % 10;
    o.travelTime = 5 + rand() % 15;
    o.price = o.distance * 25 + o.travelTime * 1.5;

    cout << "\nВаше замовлення підтверджено!\n";

    return o;
}


// --- Оплата ---
// Перевіряємо номер картки (PaymentInvalidCardNumberTest)
// Перевіряємо баланс картки (PaymentInsufficientFundsTest)
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

    // ======= ОПЛАТА КАРТКОЮ =======
    if (method == 1)
    {
        string cardNumber;
        cout << "Введіть номер картки (16 цифр): ";
        cin >> cardNumber;

        // Валідація номера картки (Тест 3)
        if (cardNumber.length() != 16  != all_of(cardNumber.begin(), cardNumber.end(), ::isdigit))
        {
            cout << "Некоректний номер картки! Додавання неможливе.\n";
            return;
        }

        // Перевірка балансу (Тест 2)
        double cardBalance;
        cout << "Введіть баланс картки: ";
        cin >> cardBalance;

        if (cardBalance < price)
        {
            cout << "Недостатньо коштів!\n";
            cout << "Оберіть інший спосіб оплати.\n";
            return;
        }

        cout << "Оплата карткою успішна.\n";
        return;
    }

    // ======= ОПЛАТА ГОТІВКОЮ =======
    else if (method == 2)
    {
        double cash;
        cout << "Введіть суму, яку ви даєте: ";

        while (!(cin >> cash) || cash <= 0)
        {
            cout << "Некоректна сума. Введіть додатне число: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        if (cash < price)
        {
            cout << "Недостатньо коштів! Оплата не виконана.\n";
            return;
        }

        double change = cash - price;
        cout << "Оплата готівкою прийнята.\n";
        cout << "Ваша решта: " << fixed << setprecision(2) << change << " грн\n";
    }
}


// --- Головна функція ---
int main()
{
   

    cout << "=== ДОДАТОК ТАКСІ ===\n";

    User user = registration(); // Реєстрація
    if (!login(user)) return 0; // Авторизація

    Order order = makeOrder();  // Оформлення поїздки
    payment(order.price);       // Оплата
    

    cout << "\nДякуємо, " << user.name << "!\n";
}
