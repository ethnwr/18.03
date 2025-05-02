#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // Для sort

using namespace std;

class Fruit {
public:
    string name;    
    double price;   
    double weight;  

    Fruit(string name, double price, double weight) : name(name), price(price), weight(weight) {}

    void print() const {
        cout << "Название: " << name << ", Цена: $" << price << ", Вес: " << weight << " кг" << endl;
    }
};

int main() {
    vector<Fruit> fruits;
    fruits.emplace_back("Яблоко", 0.50, 0.15);
    fruits.emplace_back("Банан", 0.30, 0.12);
    fruits.emplace_back("Апельсин", 0.75, 0.20);
    fruits.emplace_back("Виноград", 2.00, 0.50);
    fruits.emplace_back("Арбуз", 5.00, 5.00);
    fruits.emplace_back("Клубника", 3.00, 0.25);

    cout << "Фрукты с ценой больше $1.00:" << endl;
    for (const auto& fruit : fruits) {
        if (fruit.price > 1.00) {
            fruit.print(); 
        }
    }
    cout << endl;

    Fruit* heaviestFruit = nullptr; 
    if (!fruits.empty()) {
        heaviestFruit = &fruits[0];
        for (auto& fruit : fruits) {
            if (fruit.weight > heaviestFruit->weight) {
                heaviestFruit = &fruit; 
            }
        }
        cout << "Самый тяжелый фрукт:" << endl;
        heaviestFruit->print(); 
    }
    else
    {
        cout << "В списке нет фруктов." << endl;
    }
    cout << endl;


    sort(fruits.begin(), fruits.end(), [](const Fruit& a, const Fruit& b) {
        return a.price < b.price; 
        });

    cout << "Фрукты, отсортированные по цене (по возрастанию):" << endl;
    for (const auto& fruit : fruits) {
        fruit.print(); 
    }
    cout << endl;

    return 0;
}
