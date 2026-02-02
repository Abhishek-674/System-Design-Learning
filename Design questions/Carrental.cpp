#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// ---------------- ENUMS ----------------
enum class VehicleType { CAR, BIKE };
enum class Status { AVAILABLE, BOOKED };
enum class ReservationStatus { CREATED, CONFIRMED };

// ---------------- VEHICLE ----------------
class Vehicle {
public:
    int vehicleId;
    string number;
    Status status;

    virtual VehicleType getType() = 0;
    virtual ~Vehicle() {}
};

class Car : public Vehicle {
public:
    VehicleType getType() override {
        return VehicleType::CAR;
    }
};

class Bike : public Vehicle {
public:
    VehicleType getType() override {
        return VehicleType::BIKE;
    }
};

// ---------------- FACTORY ----------------
class VehicleFactory {
public:
    static Vehicle* createVehicle(VehicleType type) {
        if (type == VehicleType::CAR) return new Car();
        if (type == VehicleType::BIKE) return new Bike();
        return nullptr;
    }
};

// ---------------- USER ----------------
class User {
public:
    User(int id,string name,string license)
    {
         this->userId=id;
         this->name=name;
         this->license=license;
    }
    int userId;
    string name;
    string license;
};

// ---------------- LOCATION ----------------
class Location {
public:
    string city;
    string address;

    Location(string city, string address) {
        this->city = city;
        this->address = address;
    }
};

// ---------------- STORE ----------------
class Store {
public:
    int storeId;
    Location* location;
    vector<Vehicle*> vehicles;

    Store(int id, Location* loc) {
        storeId = id;
        location = loc;
    }

    void addVehicle(Vehicle* v) {
        vehicles.push_back(v);
    }

    Vehicle* getAvailableVehicle(VehicleType type) {
        for (auto v : vehicles) {
            if (v->status == Status::AVAILABLE && v->getType() == type)
                return v;
        }
        return nullptr;
    }
};

// ---------------- SINGLETON INVENTORY ----------------
class VehicleInventory {
private:
    VehicleInventory() {}
    static VehicleInventory* instance;

public:
    vector<Store*> stores;

    static VehicleInventory* getInstance() {
        if (!instance)
            instance = new VehicleInventory();
        return instance;
    }
};

VehicleInventory* VehicleInventory::instance = nullptr;

// ---------------- RESERVATION ----------------
class Reservation {
public:
    int reservationId;
    User* user;
    Vehicle* vehicle;
    Store* store;
    ReservationStatus status;

    Reservation(int id, User* u, Vehicle* v, Store* s) {
        reservationId = id;
        user = u;
        vehicle = v;
        store = s;
        status = ReservationStatus::CONFIRMED;
        vehicle->status = Status::BOOKED;
    }
};

// ---------------- PAYMENT STRATEGY ----------------
class PaymentStrategy {
public:
    virtual void pay(int amount) = 0;
    virtual ~PaymentStrategy() {}
};

class UpiPayment : public PaymentStrategy {
public:
    void pay(int amount) override {
        cout << "Paid " << amount << " using UPI\n";
    }
};

class CardPayment : public PaymentStrategy {
public:
    void pay(int amount) override {
        cout << "Paid " << amount << " using Card\n";
    }
};

class payment{
    public:
    PaymentStrategy* mth;  
    payment(PaymentStrategy* p)
    {
        mth=p;
    }
    void makepayment(int amount)
    {
         mth->pay(amount);
    }
};

// ---------------- MAIN ----------------
int main() {
    // Inventory
    VehicleInventory* inventory = VehicleInventory::getInstance();

    // Locations
    Location* blr = new Location("Bangalore", "Whitefield");
    Location* del = new Location("Delhi", "CP");

    // Stores
    Store* store1 = new Store(1, blr);
    Store* store2 = new Store(2, del);

    inventory->stores.push_back(store1);
    inventory->stores.push_back(store2);

    // Vehicles
    Vehicle* car = VehicleFactory::createVehicle(VehicleType::CAR);
    car->vehicleId = 1;
    car->number = "KA01";
    car->status = Status::AVAILABLE;

    Vehicle* bike = VehicleFactory::createVehicle(VehicleType::BIKE);
    bike->vehicleId = 2;
    bike->number = "DL01";
    bike->status = Status::AVAILABLE;

    store1->addVehicle(car);
    store2->addVehicle(bike);

    // User
    User* user = new User(1, "Abhishek", "DL123");

    // Booking
    Vehicle* bookedVehicle = store1->getAvailableVehicle(VehicleType::CAR);
    Reservation* res = new Reservation(1001, user, bookedVehicle, store1);

    cout << "Vehicle booked from store in " << store1->location->city << endl;

    // Payment
    PaymentStrategy* upi = new UpiPayment();
    payment* pay=new payment(upi);

    pay->makepayment(2000);

    return 0;
}


/*

design pattern

vehicle -> facotry design pattern
inventory-> singleton pattern
payment -> strategy pattern

IS-A

Car → Vehicle
Bike → Vehicle
UpiPayment → PaymentStrategy
CardPayment → PaymentStrategy
WalletPayment → PaymentStrategy

HAS-A

VehicleInventory HAS-A Store
Store HAS-A Vehicle
Store HAS-A Location
Reservation HAS-A User
Reservation HAS-A Vehicle
Reservation HAS-A Store
Payment HAS-A PaymentStrategy

                    <<abstract>>
+------------------------------------------------+
|                    Vehicle                     |
+------------------------------------------------+
| - vehicleId : int                              |
| - number : string                              |
| - status : Status                              |
+------------------------------------------------+
| + getType() : VehicleType                      |
+------------------------------------------------+
                        ▲
                        │ IS-A
        ┌───────────────┴───────────────┐
        │                               │
+--------------------+        +--------------------+
|        Car         |        |        Bike        |
+--------------------+        +--------------------+
|                    |        |                    |
+--------------------+        +--------------------+
| + getType()        |        | + getType()        |
+--------------------+        +--------------------+


-------------------------------------------------------------


+--------------------+          HAS-A           +--------------------+
|        Store       |------------------------▶|      Vehicle       |
+--------------------+                          +--------------------+
| - storeId : int    |                          | (List<Vehicle>)   |
| - vehicles : list  |
+--------------------+
| + addVehicle(v)    |
| + getAvailableVehicle(type) : Vehicle |
+--------------------+


+--------------------+          HAS-A           +--------------------+
|        Store       |------------------------▶|      Location      |
+--------------------+                          +--------------------+
| - storeId : int    |                          | - city : string   |
+--------------------+                          | - address : string|
                                               +--------------------+




+-------------------------+        HAS-A        +--------------------+
|    VehicleInventory     |------------------▶ |        Store       |
+-------------------------+                    +--------------------+
| - instance : static     |                    | - storeId : int    |
| - stores : List<Store>  |                    | - vehicles : list  |
+-------------------------+                    +--------------------+
| + getInstance()         |                    | + addVehicle()     |
| + getStores()           |                    | + getAvailableVehicle() |
+-------------------------+                    +--------------------+



-------------------------------------------------------------


+--------------------+          HAS-A           +--------------------+
|     Reservation    |------------------------▶|        User        |
+--------------------+                          +--------------------+
| - reservationId    |                          | - userId : int    |
| - status           |                          | - name : string   |
+--------------------+                          | - license : string|
| + confirm()        |                          +--------------------+
| + cancel()         |
+--------------------+


+--------------------+          HAS-A           +--------------------+
|     Reservation    |------------------------▶|      Vehicle       |
+--------------------+                          +--------------------+


+--------------------+          HAS-A           +--------------------+
|     Reservation    |------------------------▶|       Store        |
+--------------------+                          +--------------------+


-------------------------------------------------------------


                    <<abstract>>
+------------------------------------------------+
|               PaymentStrategy                  |
+------------------------------------------------+
| + pay(amount : double)                         |
+------------------------------------------------+
                        ▲
                        │ IS-A
        ┌───────────────┴───────────────┐
        │                               │
+--------------------+        +--------------------+
|    UpiPayment      |        |   CardPayment     |
+--------------------+        +--------------------+
| + pay(amount)      |        | + pay(amount)     |
+--------------------+        +--------------------+


                     <<Context>>
+--------------------+
|      Payment       |
+--------------------+
| - amount : double  |
| - strategy : PaymentStrategy* |
+--------------------+
| + makePayment()    |
+--------------------+


*/