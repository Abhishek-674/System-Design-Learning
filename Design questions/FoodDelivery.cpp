/*
User (Customer)
+------------------------------------------------+
| User                                           |
+------------------------------------------------+
| - userId: string                               |
| - name: string                                 |
| - phone: string                                |
| - address: string                              |
| - cart: Cart*                                  |
+------------------------------------------------+
| +addToCart(item, qty): void                    |
| +placeOrder(): Order*                          |
+------------------------------------------------+

Restaurant
+------------------------------------------------+
| Restaurant                                     |
+------------------------------------------------+
| - restaurantId: string                         |
| - name: string                                 |
| - location: string                             |
| - menu: unordered_map<string, MenuItem*>      |
+------------------------------------------------+
| +addMenuItem(item): void                       |
| +getMenu(): vector<MenuItem*>                  |
+------------------------------------------------+

MenuItem
+-----------------------------------------------+
| MenuItem                                      |
+-----------------------------------------------+
| - itemId: string                              |
| - name: string                                |
| - price: double                               |
| - isAvailable: bool                           |
+-----------------------------------------------+

Cart
+------------------------------------------------+
| Cart                                           |
+------------------------------------------------+
| - items: unordered_map<string, CartItem>      |
+------------------------------------------------+
| +addItem(item, qty): void                      |
| +removeItem(itemId): void                      |
| +getTotal(): double                            |
| +clear(): void                                 |
+------------------------------------------------+

CartItem
+-----------------------------------------------+
| CartItem                                      |
+-----------------------------------------------+
| - menuItem: MenuItem*                         |
| - quantity: int                               |
+-----------------------------------------------+
| +getSubTotal(): double                        |
+-----------------------------------------------+

Order
+------------------------------------------------+
| Order                                          |
+------------------------------------------------+
| - orderId: string                              |
| - user: User*                                  |
| - restaurant: Restaurant*                     |
| - items: vector<OrderItem>                    |
| - totalAmount: double                         |
| - status: OrderStatus                         |
+------------------------------------------------+
| +updateStatus(status): void                   |
+------------------------------------------------+

OrderItem
+-----------------------------------------------+
| OrderItem                                     |
+-----------------------------------------------+
| - itemId: string                              |
| - price: double                               |
| - quantity: int                               |
+-----------------------------------------------+

DeliveryAgent
+------------------------------------------------+
| DeliveryAgent                                 |
+------------------------------------------------+
| - agentId: string                             |
| - name: string                                |
| - isAvailable: bool                           |
+------------------------------------------------+
| +assignOrder(order): void                     |
+------------------------------------------------+

Payment
+------------------------------------------------+
| Payment                                       |
+------------------------------------------------+
| - paymentId: string                           |
| - amount: double                              |
| - status: PaymentStatus                       |
+------------------------------------------------+
| +process(): bool                              |
+------------------------------------------------+

Notification
+------------------------------------------------+
| Notification                                  |
+------------------------------------------------+
| - notificationId: string                      |
| - userId: string                              |
| - message: string                             |
| - timestamp: time_t                           |
+------------------------------------------------+

FoodDeliveryService (Singleton – Core Brain)
+----------------------------------------------------------------+
| FoodDeliveryService (Singleton)                                 |
+----------------------------------------------------------------+
| - users: unordered_map<string, User*>                          |
| - restaurants: unordered_map<string, Restaurant*>             |
| - orders: unordered_map<string, Order*>                        |
| - deliveryAgents: vector<DeliveryAgent*>                       |
+----------------------------------------------------------------+
| +placeOrder(userId): Order*                                    |
| +assignDeliveryAgent(order): void                              |
| +processPayment(order): bool                                   |
+----------------------------------------------------------------+

🔹 RELATIONSHIPS (CLEAR & FINAL)
✅ HAS-A Relationships
User HAS-A Cart
Cart HAS-A CartItem
CartItem HAS-A MenuItem

Restaurant HAS-A MenuItem
Order HAS-A OrderItem
Order HAS-A Payment
Order HAS-A DeliveryAgent

User HAS-A Order
User HAS-A Notification
*/


#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <memory>

using namespace std;

/* ===================== ENUMS ===================== */

enum class OrderStatus {
    CREATED,
    CONFIRMED,
    PREPARING,
    PICKED_UP,
    DELIVERED,
    CANCELLED
};

enum class PaymentStatus {
    PENDING,
    SUCCESS,
    FAILED
};

/* ===================== FORWARD DECLARATIONS ===================== */

class User;
class Notification;
class Order;

/* ===================== OBSERVER INTERFACES ===================== */

class IObserver {
public:
    virtual void update(Notification* notification) = 0;
    virtual ~IObserver() = default;
};

class ISubject {
public:
    virtual void registerObserver(IObserver* obs) = 0;
    virtual void removeObserver(IObserver* obs) = 0;
    virtual void notifyObservers(Notification* notification) = 0;
    virtual ~ISubject() = default;
};

/* ===================== NOTIFICATION ===================== */

class Notification {
public:
    string notificationId;
    string userId;
    string message;
    time_t timestamp;

    Notification(string uid, const string& msg)
        : userId(uid), message(msg) {
        timestamp = time(nullptr);
    }
};

/* ===================== USER ===================== */

class User : public IObserver {
private:
    string userId;
    string name;
    string phone;
    string address;
    vector<Notification> notifications;

public:
    User(string id, string name, string phone, string addr)
        : userId(id), name(name), phone(phone), address(addr) {}

    string getId() const { return userId; }
    string getName() const { return name; }

    void update(Notification* notification) override {
        notifications.push_back(*notification);
        cout << "[Notification for " << name << "]: " << notification->message << endl;
    }
};

/* ===================== MENU ITEM ===================== */

class MenuItem {
public:
    string itemId;
    string name;
    double price;
    bool isAvailable;

    MenuItem(string id, string name, double price)
        : itemId(id), name(name), price(price), isAvailable(true) {}
};

/* ===================== RESTAURANT ===================== */

class Restaurant {
private:
    string restaurantId;
    string name;
    string location;
    unordered_map<string, MenuItem*> menu;

public:
    Restaurant(string id, string name, string loc) : restaurantId(id), name(name), location(loc) {}

    void addMenuItem(MenuItem* item) {
        menu[item->itemId] = item;
    }

    vector<MenuItem*> getMenu() {
        vector<MenuItem*> items;
        for (auto& p : menu)
            items.push_back(p.second);
        return items;
    }

    string getId() const { return restaurantId; }
    string getName() const { return name; }
};

/* ===================== CART ITEM ===================== */

class CartItem {
public:
    MenuItem* menuItem;
    int quantity;

    CartItem(MenuItem* item, int qty) : menuItem(item), quantity(qty) {}

    double getSubTotal() {
        return menuItem->price * quantity;
    }
};

/* ===================== CART ===================== */

class Cart {
private:
    unordered_map<string, CartItem*> items; // key: MenuItemID

public:
    void addItem(MenuItem* item, int qty) {
        if (items.count(item->itemId))
            items[item->itemId]->quantity += qty;
        else
            items[item->itemId] = new CartItem(item, qty);
    }

    void removeItem(const string& itemId) {
        if (items.count(itemId)) {
            delete items[itemId];
            items.erase(itemId);
        }
    }

    double getTotal() {
        double total = 0;
        for (auto& p : items)
            total += p.second->getSubTotal();
        return total;
    }

    vector<CartItem*> getItems() {
        vector<CartItem*> v;
        for (auto& p : items)
            v.push_back(p.second);
        return v;
    }

    void clear() {
        for (auto& p : items)
            delete p.second;
        items.clear();
    }

    ~Cart() {
        clear();
    }
};

/* ===================== ORDER ITEM ===================== */

class OrderItem {
public:
    string itemId;
    double price;
    int quantity;

    OrderItem(string id, double price, int qty)
        : itemId(id), price(price), quantity(qty) {}
};

/* ===================== PAYMENT ===================== */

class Payment {
public:
    string paymentId;
    double amount;
    PaymentStatus status;

    Payment(double amt) : amount(amt), status(PaymentStatus::PENDING) {}

    bool process() {
        // Simplified payment processing
        status = PaymentStatus::SUCCESS;
        return true;
    }
};

/* ===================== DELIVERY AGENT ===================== */

class DeliveryAgent {
public:
    string agentId;
    string name;
    bool isAvailable;

    DeliveryAgent(string id, string name) : agentId(id), name(name), isAvailable(true) {}

    void assignOrder(Order* order);
};

/* ===================== ORDER ===================== */

class Order : public ISubject {
private:
    string orderId;
    User* user;
    Restaurant* restaurant;
    vector<OrderItem> items;
    double totalAmount;
    OrderStatus status;
    Payment* payment;
    vector<IObserver*> observers;

public:
    Order(string id, User* u, Restaurant* r)
        : orderId(id), user(u), restaurant(r), totalAmount(0), status(OrderStatus::CREATED), payment(nullptr) {}

    void addObserver(IObserver* obs) override {
        observers.push_back(obs);
    }

    void removeObserver(IObserver* obs) override {
        observers.erase(remove(observers.begin(), observers.end(), obs), observers.end());
    }

    void notifyObservers(Notification* notification) override {
        for (auto obs : observers) {
            obs->update(notification);
        }
    }

    void addItem(MenuItem* menuItem, int qty) {
        items.push_back(OrderItem(menuItem->itemId, menuItem->price, qty));
        totalAmount += menuItem->price * qty;
    }

    double getTotal() const { return totalAmount; }

    void setPayment(Payment* pay) { payment = pay; }

    void updateStatus(OrderStatus s) {
        status = s;
        Notification* notif = new Notification(user->getId(), "Order " + orderId + " status updated.");
        notifyObservers(notif);
    }

    string getId() const { return orderId; }
};

/* ===================== DELIVERY AGENT ===================== */

void DeliveryAgent::assignOrder(Order* order) {
    if (isAvailable) {
        cout << "Delivery agent " << name << " assigned to order " << order->getId() << endl;
        isAvailable = false;
        order->updateStatus(OrderStatus::PICKED_UP);
    }
}

/* ===================== FOOD DELIVERY SERVICE (SINGLETON) ===================== */

class FoodDeliveryService {
private:
    unordered_map<string, User*> users;
    unordered_map<string, Restaurant*> restaurants;
    unordered_map<string, Order*> orders;
    vector<DeliveryAgent*> deliveryAgents;

    FoodDeliveryService() {}

public:
    static FoodDeliveryService& getInstance() {
        static FoodDeliveryService instance;
        return instance;
    }

    void addUser(User* user) { users[user->getId()] = user; }
    void addRestaurant(Restaurant* restaurant) { restaurants[restaurant->getId()] = restaurant; }
    void addDeliveryAgent(DeliveryAgent* agent) { deliveryAgents.push_back(agent); }

    Order* placeOrder(const string& userId, const string& restaurantId, Cart* cart) {
        if (!users.count(userId) || !restaurants.count(restaurantId)) return nullptr;

        User* user = users[userId];
        Restaurant* restaurant = restaurants[restaurantId];

        string orderId = "order_" + to_string(rand());
        Order* order = new Order(orderId, user, restaurant);
        order->addObserver(user);

        for (auto item : cart->getItems()) {
            order->addItem(item->menuItem, item->quantity);
        }

        Payment* payment = new Payment(order->getTotal());
        payment->process();
        order->setPayment(payment);

        orders[orderId] = order;

        Notification* notif = new Notification(userId, "Order " + orderId + " placed successfully!");
        order->notifyObservers(notif);

        cart->clear(); // clear user's cart

        assignDeliveryAgent(order);

        return order;
    }

    void assignDeliveryAgent(Order* order) {
        for (auto agent : deliveryAgents) {
            if (agent->isAvailable) {
                agent->assignOrder(order);
                return;
            }
        }
        cout << "No delivery agent available for order " << order->getId() << endl;
    }
};

/* ===================== MAIN ===================== */

int main() {
    FoodDeliveryService& service = FoodDeliveryService::getInstance();

    // Users
    User* alice = new User("1", "Alice", "123456", "123 Street");
    User* bob = new User("2", "Bob", "654321", "456 Street");
    service.addUser(alice);
    service.addUser(bob);

    // Restaurants
    Restaurant* pizzaPlace = new Restaurant("r1", "Pizza Place", "Downtown");
    pizzaPlace->addMenuItem(new MenuItem("m1", "Margherita", 5.0));
    pizzaPlace->addMenuItem(new MenuItem("m2", "Pepperoni", 6.5));
    service.addRestaurant(pizzaPlace);

    // Delivery agents
    service.addDeliveryAgent(new DeliveryAgent("d1", "Dave"));
    service.addDeliveryAgent(new DeliveryAgent("d2", "Eve"));

    // User cart
    Cart* aliceCart = new Cart();
    aliceCart->addItem(pizzaPlace->getMenu()[0], 2); // 2 Margherita
    aliceCart->addItem(pizzaPlace->getMenu()[1], 1); // 1 Pepperoni

    // Place order
    Order* order = service.placeOrder("1", "r1", aliceCart);

    delete alice;
    delete bob;

    return 0;
}
