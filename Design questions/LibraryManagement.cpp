#include <iostream>
#include <vector>
using namespace std;

/* ================= BOOK ================= */

class Book {
public:
    string title;
    string author;
    int totalCopies;
    int availableCopies;

    Book(string title, string author, int copies) {
        this->title = title;
        this->author = author;
        totalCopies = copies;
        availableCopies = copies;
    }

    bool isAvailable() {
        return availableCopies > 0;
    }

    void borrowBook() {
        availableCopies--;
    }

    void returnBook() {
        availableCopies++;
    }
};

/* ================= USER ================= */

class User {
public:
    string userName;
    int userId;
    vector<Book*> borrowedBooks;

    User(int userId, string userName) {
        this->userId = userId;
        this->userName = userName;
    }

    void addBorrowedBook(Book* book) {
        borrowedBooks.push_back(book);
    }

    void removeBorrowedBook(Book* book) {
        for (int i = 0; i < borrowedBooks.size(); i++) {
            if (borrowedBooks[i] == book) {
                swap(borrowedBooks[i], borrowedBooks.back());
                borrowedBooks.pop_back();
                break;
            }
        }
    }
};

/* ================= FINE STRATEGY ================= */

class FineStrategy {
public:
    virtual int calculateFine(int days) = 0;
    virtual ~FineStrategy() {}
};

class StudentFineStrategy : public FineStrategy {
public:
    int calculateFine(int days) override {
        return 2 * days;
    }
};

class HandicappedFineStrategy : public FineStrategy {
public:
    int calculateFine(int days) override {
        return days / 2;
    }
};

/* ================= PAYMENT STRATEGY ================= */

class PaymentStrategy {
public:
    virtual void payAmount() = 0;
    virtual ~PaymentStrategy() {}
};

class UpiPayment : public PaymentStrategy {
public:
    void payAmount() override {
        cout << "Payment completed using UPI" << endl;
    }
};

class CashPayment : public PaymentStrategy {
public:
    void payAmount() override {
        cout << "Payment completed using Cash" << endl;
    }
};

/* ================= LIBRARY (SINGLETON) ================= */

class LibraryManagementSystem {
private:
    static LibraryManagementSystem* instance;
    LibraryManagementSystem() {}

public:
    FineStrategy* fineStrategy = nullptr;
    PaymentStrategy* paymentStrategy = nullptr;

    vector<User*> users;
    vector<Book*> books;

    static LibraryManagementSystem* getInstance() {
        if (!instance)
            instance = new LibraryManagementSystem();
        return instance;
    }

    void setFineStrategy(FineStrategy* fineStrategy) {
        this->fineStrategy = fineStrategy;
    }

    void setPaymentStrategy(PaymentStrategy* paymentStrategy) {
        this->paymentStrategy = paymentStrategy;
    }

    void addUser(User* user) {
        users.push_back(user);
        cout << "User added: " << user->userName << endl;
    }

    void addBook(Book* book) {
        books.push_back(book);
        cout << "Book added: " << book->title << endl;
    }

    void borrowBook(User* user, Book* book) {
        if (book->isAvailable()) {
            book->borrowBook();
            user->addBorrowedBook(book);
            cout << user->userName << " borrowed " << book->title << endl;
        } else {
            cout << "Book not available" << endl;
        }
    }

    void calculateFine(int overdueDays) {
        if (fineStrategy)
            cout << "Fine Amount = " 
                 << fineStrategy->calculateFine(overdueDays) << endl;
    }

    void processPayment() {
        if (paymentStrategy)
            paymentStrategy->payAmount();
    }
};

LibraryManagementSystem* LibraryManagementSystem::instance = nullptr;

/* ================= MAIN ================= */

int main() {
    LibraryManagementSystem* library = LibraryManagementSystem::getInstance();

    library->addBook(new Book("C++", "Bjarne", 3));
    library->addBook(new Book("Java", "James Gosling", 2));

    User* user1 = new User(1, "Abhishek");
     User* user2 = new User(1, "Abhishek");
      User* user3 = new User(1, "Abhishek");
       User* user4 = new User(1, "Abhishek");
    library->addUser(user1);
     library->addUser(user2);
      library->addUser(user3);
       library->addUser(user4);
        // library->addUser(user1);
    

    library->borrowBook(user1, library->books[1]);
    library->borrowBook(user1, library->books[1]);
    library->borrowBook(user1, library->books[1]);
    library->borrowBook(user1, library->books[1]);

    library->setFineStrategy(new StudentFineStrategy());
    library->calculateFine(5);

    library->setPaymentStrategy(new UpiPayment());
    library->processPayment();

    return 0;
}
 

/*
High-Level Design (OOP Based)
Main Entities

Book

User (Member)

Librarian

Library

Transaction (Borrow / Return)

Fine

Report

2️⃣ Class Responsibilities (Simple English)
📘 Book

Stores book details

Tracks available copies

Tracks category / genre

👤 User (Member)

Stores user info

Tracks borrowed books

Enforces borrow limit & days limit

🧑‍💼 Librarian

Adds / removes books

Adds / removes users

Generates reports

🏛 Library

Central controller

Manages books, users, transactions

Applies rules (limits, fines)

🔁 Transaction

Handles borrow, return, renew

Stores issue date & due date

💰 Fine

Calculates fine for overdue books

📊 Report

Generates usage & inventory reports
*/