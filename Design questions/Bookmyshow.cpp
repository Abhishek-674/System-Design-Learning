#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <mutex>


using namespace std;

/* ================= USER ================= */
class User {
public:
    int id;
    string name;
    User(int id, string name) : id(id), name(name) {}
};

/* ================= SEAT ================= */
class Seat {
public:
    int seatId;
    bool isBooked;

    Seat(int id) : seatId(id), isBooked(false) {}
};

/* ================= SCREEN ================= */
class Screen {
public:
    int screenId;
    vector<Seat*> seats;

    Screen(int id, int seatCount) : screenId(id) {
        for (int i = 1; i <= seatCount; i++)
            seats.push_back(new Seat(i));
    }
};

/* ================= SHOW ================= */
class Show {
public:
    int showId;
    string time;
    Screen* screen;
    mutex seatLock;

    Show(int id, string time, Screen* screen)
        : showId(id), time(time), screen(screen) {}
};

/* ================= MOVIE ================= */
class Movie {
public:
    string name;
    vector<Show*> shows;

    Movie(string name) : name(name) {}
};

/* ================= THEATRE ================= */
class Theatre {
public:
    string name;
    vector<Movie*> movies;

    Theatre(string name) : name(name) {}
};

/* ================= CITY ================= */
class City {
public:
    string name;
    vector<Theatre*> theatres;

    City(string name) : name(name) {}
};

/* ================= PAYMENT STRATEGY ================= */
class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentStrategy() {}
};

class UpiPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid ₹" << amount << " using UPI\n";
    }
};

class CardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paid ₹" << amount << " using Card\n";
    }
};

/* ================= PAYMENT ================= */
class Payment {
    PaymentStrategy* strategy;
public:
    Payment(PaymentStrategy* strategy) : strategy(strategy) {}
    void makePayment(double amount) {
        strategy->pay(amount);
    }
};

/* ================= BOOKING ================= */
class Booking {
public:
    User* user;
    Show* show;
    vector<Seat*> bookedSeats;

    Booking(User* user, Show* show) : user(user), show(show) {}
};

/* ================= BOOKING MANAGER (SINGLETON) ================= */
class BookingManager {
private:
    static BookingManager* instance;
    mutex bookingMutex;

    BookingManager() {}

public:
    static BookingManager* getInstance() {
        static BookingManager instance;
        return &instance;
    }

    bool bookSeats(User* user, Show* show, vector<int> seatIds) {
        lock_guard<mutex> lock(show->seatLock);

        vector<Seat*> selectedSeats;
        for (int id : seatIds) {
            Seat* seat = show->screen->seats[id - 1];
            if (seat->isBooked) {
                cout << "Seat " << id << " already booked\n";
                return false;
            }
            selectedSeats.push_back(seat);
        }

        for (Seat* seat : selectedSeats)
            seat->isBooked = true;

        cout << "Booking successful for " << user->name << "\n";
        return true;
    }
};

/* ================= MAIN ================= */
int main() {

    // City
    City* city = new City("Bangalore");

    // Theatre
    Theatre* theatre = new Theatre("PVR");
    city->theatres.push_back(theatre);

    // Movie
    Movie* movie = new Movie("Interstellar");
    theatre->movies.push_back(movie);

    // Screen
    Screen* screen = new Screen(1, 10);

    // Show
    Show* show = new Show(101, "7:00 PM", screen);
    movie->shows.push_back(show);

    // Users
    User* u1 = new User(1, "Abhishek");
    User* u2 = new User(2, "Rahul");

    // Booking (Simultaneous Simulation)
    BookingManager* manager = BookingManager::getInstance();

    thread t1([&]() {
        manager->bookSeats(u1, show, {1, 2});
    });

    thread t2([&]() {
        manager->bookSeats(u2, show, {2, 3});
    });

    t1.join();
    t2.join();

    // Payment
    Payment payment(new UpiPayment());
    payment.makePayment(500);

    return 0;
}

/*

Relationships (Exactly as You Asked)
HAS-A

City HAS-A Theatre

Theatre HAS-A Movie

Movie HAS-A Show

Show HAS-A Screen

Screen HAS-A Seat

Booking HAS-A User

Booking HAS-A Show

Payment HAS-A PaymentStrategy

IS-A

UpiPayment IS-A PaymentStrategy

CardPayment IS-A PaymentStrategy

+--------------------+
|       City         |
+--------------------+
| - cityId           |
| - name             |
| - theatres : List<Theatre> |
+--------------------+
| + getTheatres()    |
+--------------------+
          |
          | HAS-A
          v
+--------------------+
|      Theatre       |
+--------------------+
| - theatreId        |
| - name             |
| - movies : List<Movie> |
+--------------------+
| + getMovies()      |
+--------------------+
          |
          | HAS-A
          v
+--------------------+
|       Movie        |
+--------------------+
| - movieId          |
| - title            |
| - duration         |
| - language         |
| - shows : List<Show> |
+--------------------+
| + getShows()       |
+--------------------+
          |
          | HAS-A
          v
+--------------------+
|        Show        |
+--------------------+
| - showId           |
| - startTime        |
| - price            |
| - screen : Screen  |
+--------------------+
| + getAvailableSeats() |
+--------------------+
          |
          | HAS-A
          v
+--------------------+
|       Screen       |
+--------------------+
| - screenId         |
| - seats : List<Seat> |
+--------------------+
| + getSeats()       |
+--------------------+
          |
          | HAS-A
          v
+--------------------+
|        Seat        |
+--------------------+
| - seatId           |
| - row              |
| - number           |
| - status           |
+--------------------+
| + bookSeat()       |
+--------------------+


+--------------------+
|        User        |
+--------------------+
| - userId           |
| - name             |
| - email            |
| - phone            |
+--------------------+
| + bookTicket()     |
+--------------------+


+--------------------+
|      Booking       |
+--------------------+
| - bookingId        |
| - user : User      |
| - show : Show      |
| - seats : List<Seat> |
| - status           |
+--------------------+
| + confirmBooking() |
| + cancelBooking()  |
+--------------------+
        |        |
        | HAS-A  | HAS-A
        v        v
      User     Show


+----------------------------+
|        Payment             |
+----------------------------+
| - amount                   |
| - strategy : PaymentStrategy |
+----------------------------+
| + makePayment()            |
+----------------------------+
              |
              | HAS-A
              v
     +-----------------------+
     | <<interface>>         |
     |   PaymentStrategy     |
     +-----------------------+
     | + pay(amount)         |
     +-----------------------+
        ^           ^
        | IS-A      | IS-A
+----------------+  +----------------+
|  UpiPayment    |  | CardPayment    |
+----------------+  +----------------+
| + pay()        |  | + pay()        |
+----------------+  +----------------+


*/


