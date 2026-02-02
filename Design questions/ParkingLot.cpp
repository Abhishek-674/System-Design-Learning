#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

/* ================= VEHICLE ================= */

class Vehicle {
public:
    string number;
    virtual string getType() = 0;
    virtual ~Vehicle() {}
};

class Car : public Vehicle {
public:
    Car(string num) { number = num; }
    string getType() { return "Car"; }
};

class Bike : public Vehicle {
public:
    Bike(string num) { number = num; }
    string getType() { return "Bike"; }
};

/* ================= VEHICLE FACTORY ================= */

class VehicleFactory {
public:
    static Vehicle* createVehicle(string type, string number) {
        if (type == "car") return new Car(number);
        if (type == "bike") return new Bike(number);
        return nullptr;
    }
};

/* ================= PARKING SLOT ================= */

class ParkingSlot {
public:
    int floorNo;
    int slotNo;
    bool occupied;
    Vehicle* vehicle;

    ParkingSlot(int f, int s) {
        floorNo = f;
        slotNo = s;
        occupied = false;
        vehicle = nullptr;
    }

    void park(Vehicle* v) {
        vehicle = v;
        occupied = true;
    }

    void unpark() {
        vehicle = nullptr;
        occupied = false;
    }
};

/* ================= PARKING FLOOR ================= */

class ParkingFloor {
public:
    vector<ParkingSlot*> slots;

    ParkingFloor(int floorNo, int totalSlots) {
        for (int i = 0; i < totalSlots; i++) {
            slots.push_back(new ParkingSlot(floorNo, i));
        }
    }
};

/* ================= STRATEGY ================= */

class SlotAllocationStrategy {
public:
    virtual ParkingSlot* getSlot(
        vector<ParkingFloor*>& floors) = 0;
    virtual ~SlotAllocationStrategy() {}
};

/* ---- Strategy 1: First Available ---- */
class FirstAvailableStrategy : public SlotAllocationStrategy {
public:
    ParkingSlot* getSlot(vector<ParkingFloor*>& floors) {
        for (auto floor : floors) {
            for (auto slot : floor->slots) {
                if (!slot->occupied)
                    return slot;
            }
        }
        return nullptr;
    }
};

/* ---- Strategy 2: Nearest Entry ---- */
class NearestEntryStrategy : public SlotAllocationStrategy {
public:
    ParkingSlot* getSlot(vector<ParkingFloor*>& floors) {
        ParkingSlot* best = nullptr;

        for (auto floor : floors) {
            for (auto slot : floor->slots) {
                if (!slot->occupied) {
                    best = slot;
                    break;
                }
            }
            if (best) break;
        }
        return best;
    }
};

/* ================= PARKING LOT (SINGLETON) ================= */

class ParkingLot {
private:
    static ParkingLot* instance;
    vector<ParkingFloor*> floors;
    unordered_map<string, ParkingSlot*> parkedMap;
    SlotAllocationStrategy* strategy;

    ParkingLot() {
        strategy = new FirstAvailableStrategy();
        for (int i = 0; i < 2; i++) {
            floors.push_back(new ParkingFloor(i, 5));
        }
    }

public:
    static ParkingLot* getInstance() {
        if (!instance)
            instance = new ParkingLot();
        return instance;
    }

    void setStrategy(SlotAllocationStrategy* s) {
        strategy = s;
        cout << "Parking strategy changed\n";
    }

    void parkVehicle(Vehicle* v) {
        ParkingSlot* slot = strategy->getSlot(floors);
        if (!slot) {
            cout << "Parking Full\n";
            return;
        }
        slot->park(v);
        parkedMap[v->number] = slot;

        cout << v->number << " parked at Floor "
             << slot->floorNo << ", Slot "
             << slot->slotNo << endl;
    }

    void unparkVehicle(string number) {
        if (parkedMap.find(number) == parkedMap.end()) {
            cout << "Vehicle not found\n";
            return;
        }
        parkedMap[number]->unpark();
        parkedMap.erase(number);
        cout << number << " exited parking\n";
    }
};

ParkingLot* ParkingLot::instance = nullptr;

/* ================= MAIN ================= */

int main() {
    ParkingLot* lot = ParkingLot::getInstance();

    Vehicle* v1 = VehicleFactory::createVehicle("car", "CAR101");
    Vehicle* v2 = VehicleFactory::createVehicle("bike", "BIKE202");
    Vehicle* v3 = VehicleFactory::createVehicle("car", "CAR303");

    // Default Strategy: First Available
    lot->parkVehicle(v1);
    lot->parkVehicle(v2);

    // Switch Strategy at runtime
    lot->setStrategy(new NearestEntryStrategy());
    lot->parkVehicle(v3);

    lot->unparkVehicle("BIKE202");

    return 0;
}


/*

Design Patterns Used
Factory → Vehicle creation
Singleton → ParkingLot
Strategy → Slot allocation


IS-A

Car IS-A Vehicle

Bike IS-A Vehicle

HAS-A

ParkingLot HAS-A ParkingFloor

ParkingFloor HAS-A ParkingSlot

ParkingSlot HAS-A Vehicle



                  +----------------------+
                  |   ParkingLot         |  <<Singleton>>
                  +----------------------+
                  | -floors: vector<ParkingFloor*> |
                  | -strategy: SlotAllocationStrategy* |
                  +----------------------+
                  | +getInstance(): ParkingLot*      |
                  | +addVehicle(Vehicle*)           |
                  | +removeVehicle(Vehicle*)        |
                  | +setStrategy(SlotAllocationStrategy*) |
                  +----------------------+
                               |
                               | has
                               v
                  +----------------------+
                  |   ParkingFloor       |
                  +----------------------+
                  | -slots: vector<ParkingSlot*> |
                  +----------------------+
                  | +getSlots()                     |
                  +----------------------+
                               |
                               | has
                               v
                  +----------------------+
                  |   ParkingSlot        |
                  +----------------------+
                  | -isOccupied: bool    |
                  | -vehicleParked: Vehicle* |
                  | -id, id2: int        |
                  +----------------------+
                  | +parkVehicle(Vehicle*) |
                  | +unparkVehicle()       |
                  +----------------------+

                  +----------------------+
                  |      Vehicle         |  <<Abstract>>
                  +----------------------+
                  | -type: string        |
                  | -number: string      |
                  +----------------------+
                  | +getType(): string   |
                  +----------------------+
                               ^
                               |
           ---------------------------------------
           |                                     |
   +---------------+                     +---------------+
   |      Car      |                     |      Bike     |
   +---------------+                     +---------------+
   | +getType()    |                     | +getType()    |
   +---------------+                     +---------------+

                  +----------------------+
                  | SlotAllocationStrategy |  <<Strategy>>
                  +----------------------+
                  | +getSlot(floors): ParkingSlot* |
                  +----------------------+
                               ^
                               |
           ---------------------------------------
           |                                     |
+-------------------------+         +-------------------------+
| FirstAvailableStrategy   |         | NearestEntryStrategy    |
+-------------------------+         +-------------------------+
| +getSlot(floors)        |         | +getSlot(floors)        |
+-------------------------+         +-------------------------+



*/









