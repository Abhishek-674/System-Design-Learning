#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* ===================== Item ===================== */
class Item {
public:
    string name;
    int price;

    Item(string name, int price) : name(name), price(price) {}
};

/* ===================== Location ===================== */
class Location {
public:
    string city;
    int pincode;

    Location(string city, int pincode) : city(city), pincode(pincode) {}
};

/* ===================== Restaurant ===================== */
class Restaurant {
public:
    string name;
    Location location;
    vector<Item*> menu;

    Restaurant(string name, Location location)
        : name(name), location(location) {}

    void addItem(Item* item) {
        menu.push_back(item);
    }
};

/* ===================== CartItem ===================== */
class CartItem {
public:
    Item* item;
    int quantity;

    CartItem(Item* item, int quantity)
        : item(item), quantity(quantity) {}

    int getPrice() {
        return item->price * quantity;
    }
};

/* ===================== User ===================== */
class User {
public:
    string name;
    vector<CartItem*> cart;

    User(string name) : name(name) {}

    void addToCart(Item* item, int qty) {
        cart.push_back(new CartItem(item, qty));
    }

    int getCartTotal() {
        int total = 0;
        for (auto c : cart)
            total += c->getPrice();
        return total;
    }

    void clearCart() {
        cart.clear();
    }
};

/* ===================== Order ===================== */
class Order {
public:
    User* user;
    Restaurant* restaurant;
    vector<CartItem*> items;
    int totalAmount;

    Order(User* user, Restaurant* restaurant, vector<CartItem*> items)
        : user(user), restaurant(restaurant), items(items) {

        totalAmount = 0;
        for (auto i : items)
            totalAmount += i->getPrice();
    }

    void showOrder() {
        cout << "\nOrder Details\n";
        cout << "User: " << user->name << endl;
        cout << "Restaurant: " << restaurant->name << endl;
        cout << "Items:\n";
        for (auto i : items) {
            cout << "- " << i->item->name
                 << " x " << i->quantity << endl;
        }
        cout << "Total Amount: " << totalAmount << endl;
    }
};

/* ===================== Search Strategy ===================== */
class SearchStrategy {
public:
    virtual vector<Restaurant*> search(
        vector<Restaurant*>& restaurants,
        string query) = 0;

    virtual ~SearchStrategy() {}
};

/* ---------- Search by Restaurant Name ---------- */
class SearchByName : public SearchStrategy {
public:
    vector<Restaurant*> search(
        vector<Restaurant*>& restaurants,
        string query) override {

        vector<Restaurant*> result;
        for (auto r : restaurants) {
            if (r->name.find(query) != string::npos)
                result.push_back(r);
        }
        return result;
    }
};

/* ---------- Search by Item Name ---------- */
class SearchByItem : public SearchStrategy {
public:
    vector<Restaurant*> search(
        vector<Restaurant*>& restaurants,
        string query) override {

        vector<Restaurant*> result;
        for (auto r : restaurants) {
            for (auto i : r->menu) {
                if (i->name.find(query) != string::npos) {
                    result.push_back(r);
                    break;
                }
            }
        }
        return result;
    }
};

/* ---------- Search by Location ---------- */
class SearchByLocation : public SearchStrategy {
public:
    vector<Restaurant*> search(
        vector<Restaurant*>& restaurants,
        string query) override {

        vector<Restaurant*> result;
        for (auto r : restaurants) {
            if (r->location.city == query)
                result.push_back(r);
        }
        return result;
    }
};

/* ===================== App (Context) ===================== */
class FoodDeliveryApp {
public:
    vector<User*> users;
    vector<Restaurant*> restaurants;
    SearchStrategy* searchStrategy;

    void addUser(User* u) {
        users.push_back(u);
    }

    void addRestaurant(Restaurant* r) {
        restaurants.push_back(r);
    }

    void setSearchStrategy(SearchStrategy* s) {
        searchStrategy = s;
    }

    vector<Restaurant*> search(string query) {
        // if (!searchStrategy) return {};
        return searchStrategy->search(restaurants, query);
    }

    Order* placeOrder(User* user, Restaurant* restaurant) {
        Order* order = new Order(user, restaurant, user->cart);
        user->clearCart();
        return order;
    }
};

/* ===================== main ===================== */
int main() {
    FoodDeliveryApp app;

    // Locations
    Location delhi("Delhi", 110001);
    Location bangalore("Bangalore", 560001);

    // Restaurants
    Restaurant* r1 = new Restaurant("Pizza Hub", delhi);
    Restaurant* r2 = new Restaurant("Burger King", bangalore);

    Item* pizza = new Item("Pizza", 300);
    Item* burger = new Item("Burger", 150);

    r1->addItem(pizza);
    r2->addItem(burger);

    app.addRestaurant(r1);
    app.addRestaurant(r2);

    // User
    User* user = new User("Abhishek");
    app.addUser(user);

    // -------- Search by Item --------
    app.setSearchStrategy(new SearchByItem());
    auto result = app.search("Pizza");

    cout << "Search Results:\n";
    for (auto r : result)
        cout << r->name << endl;

    // -------- Order Flow --------
    user->addToCart(pizza, 2);
    Order* order = app.placeOrder(user, r1);
    order->showOrder();

    return 0;
}


/*
+-----------------------------+
|            App              |
+-----------------------------+
| - users : vector<User*>     |
| - restaurants : vector<Restaurant*> |
| - searchStrategy : SearchStrategy* |
+-----------------------------+
| + addUser(u: User*)         |
| + addRestaurant(r: Restaurant*) |
| + setSearchStrategy(s: SearchStrategy*) |
| + search(query: string) : vector<Restaurant*> |
| + placeOrder(u: User*, r: Restaurant*) : Order* |
+-----------------------------+
              |
              | uses
              v
+----------------------------------+
|        SearchStrategy             | <<interface>>
+----------------------------------+
| + search(rests: vector<Restaurant*>, |
|           query: string) : vector<Restaurant*> |
+----------------------------------+
        ^                 ^                   ^
        |                 |                   |
+------------------+  +------------------+  +------------------+
| SearchByName     |  | SearchByItem     |  | SearchByLocation |
+------------------+  +------------------+  +------------------+
| + search(...)    |  | + search(...)    |  | + search(...)    |
+------------------+  +------------------+  +------------------+

+---------------------+          +---------------------+
|        User         |          |     Restaurant      |
+---------------------+          +---------------------+
| - name : string     |          | - name : string     |
| - cart : vector<CartItem*> |   | - menu : vector<Item*> |
+---------------------+          | - location : Location |
| + addToCart(item: Item*, qty:int) |
| + getCartTotal() : int          |
| + clearCart()                   |
+---------------------+          +---------------------+
        | 1                                   | 1
        | has                                 | has
        | *                                   | *
+---------------------+          +---------------------+
|      CartItem       |          |        Item         |
+---------------------+          +---------------------+
| - item : Item*      |          | - name : string     |
| - quantity : int    |          | - price : int       |
+---------------------+          +---------------------+
| + getPrice() : int  |
+---------------------+

+---------------------+
|        Order        |
+---------------------+
| - user : User*      |
| - restaurant : Restaurant* |
| - items : vector<CartItem*> |
| - totalAmount : int |
+---------------------+
| + showOrder()       |
+---------------------+

+---------------------+
|      Location       |
+---------------------+
| - city : string     |
| - pincode : int     |
+---------------------+
        ^
        |
        | 1
+---------------------+
|     Restaurant      |
+---------------------+


*/