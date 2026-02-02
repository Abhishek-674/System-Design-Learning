#include<iostream>
#include<vector>
#include<map>
#include<unordered_map>
using namespace std;

enum class SplitType {
    EQUAL,
    EXACT,
    PERCENTAGE
};

class User {
public:
    string userId;
    string name;
    string email;

    User(string id, string n, string e) {
        userId = id;
        name = n;
        email = e;
    }
};
class Group {
public:
    string groupId;
    string name;
    vector<User*> members;

    Group(string id, string n) {
        groupId = id;
        name = n;
    }

    void addMember(User* user) {
        members.push_back(user);
    }
};

class SplitStrategy {
public:
    virtual unordered_map<User*, double>
    calculate(double amount, vector<User*> users,
              unordered_map<User*, double> values) = 0;
};

class EqualSplit : public SplitStrategy {
public:
    unordered_map<User*, double>
    calculate(double amount, vector<User*> users,
              unordered_map<User*, double>) override {
        unordered_map<User*, double> result;
        double share = amount / users.size();
        for (auto user : users)
            result[user] = share;
        return result;
    }
};
class ExactSplit : public SplitStrategy {
public:
    unordered_map<User*, double>
    calculate(double, vector<User*>,
              unordered_map<User*, double> values) override {
        return values;
    }
};
class PercentageSplit : public SplitStrategy {
public:
    unordered_map<User*, double>
    calculate(double amount, vector<User*>,
              unordered_map<User*, double> values) override {
        unordered_map<User*, double> result;
        for (auto &it : values)
            result[it.first] = (it.second / 100.0) * amount;
        return result;
    }
};
class Expense {
public:
    string expenseId;
    string description;
    double amount;
    User* paidBy;
    Group* group;
    unordered_map<User*, double> splits;

    Expense(string id, string desc, double amt,
            User* payer, Group* grp) {
        expenseId = id;
        description = desc;
        amount = amt;
        paidBy = payer;
        group = grp;
    }
};
class ExpenseFactory {
public:
    static SplitStrategy* getStrategy(SplitType type) {
        if (type == SplitType::EQUAL) return new EqualSplit();
        if (type == SplitType::EXACT) return new ExactSplit();
        return new PercentageSplit();
    }
};

class Transaction {
public:
    User* from;
    User* to;
    double amount;

    Transaction(User* f, User* t, double a) {
        from = f;
        to = t;
        amount = a;
    }
};
class SplitwiseService {
private:
    unordered_map<User*, unordered_map<User*, double>> balance;
    vector<Transaction> transactions;

    static SplitwiseService* instance;
    SplitwiseService() {}

public:
    static SplitwiseService* getInstance() {
        if (!instance)
            instance = new SplitwiseService();
        return instance;
    }

    void addExpense(Expense* expense, SplitType type,
                    unordered_map<User*, double> splitValues) {

        SplitStrategy* strategy = ExpenseFactory::getStrategy(type);
        auto shares = strategy->calculate(
            expense->amount, expense->group->members, splitValues);

        for (auto &it : shares) {
            User* user = it.first;
            double share = it.second;

            if (user != expense->paidBy) {
                balance[user][expense->paidBy] += share;
                balance[expense->paidBy][user] -= share;
            }
        }
    }

    void showBalances(User* user) {
        for (auto &it : balance[user]) {
            if (it.second > 0)
                cout << user->name << " owes "
                     << it.first->name << " : " << it.second << endl;
        }
    }

    void settle(User* from, User* to, double amount) {
        balance[from][to] -= amount;
        balance[to][from] += amount;
        transactions.emplace_back(from, to, amount);
    }

    void showTransactions() {
        for (auto &t : transactions)
            cout << t.from->name << " paid "
                 << t.to->name << " : " << t.amount << endl;
    }
};

SplitwiseService* SplitwiseService::instance = nullptr;

int main() {
    User* u1 = new User("1", "Abhishek", "a@gmail.com");
    User* u2 = new User("2", "Rahul", "r@gmail.com");
    User* u3 = new User("3", "Amit", "am@gmail.com");

    Group* g1 = new Group("g1", "Trip");
    g1->addMember(u1);
    g1->addMember(u2);
    g1->addMember(u3);

    SplitwiseService* service = SplitwiseService::getInstance();

    Expense* e1 = new Expense("e1", "Hotel", 3000, u1, g1);
    service->addExpense(e1, SplitType::EQUAL, {});

    cout << "\nBalances:\n";
    service->showBalances(u2);
    service->showBalances(u3);

    cout << "\nSettlement:\n";
    service->settle(u2, u1, 1000);

    cout << "\nUpdated Balances:\n";
    service->showBalances(u2);

    cout << "\nTransactions:\n";
    service->showTransactions();

    return 0;
}

/*
+-------------------+
|       User        |
+-------------------+
| - userId          |
| - name            |
| - email           |
| - phone           |
+-------------------+
| +updateProfile()  |
+-------------------+

+-------------------+
|       Group       |
+-------------------+
| - groupId         |
| - groupName       |
| - members         |
+-------------------+
| +addMember()      |
| +removeMember()   |
+-------------------+

+-------------------+
|      Expense      |
+-------------------+
| - expenseId       |
| - description     |
| - amount          |
| - paidBy          |
| - group           |
| - splitType       |
| - shares          |
+-------------------+
| +calculateShares()|
+-------------------+

+-------------------+
|    Transaction    |
+-------------------+
| - fromUser        |
| - toUser          |
| - amount          |
| - description     |
+-------------------+

+-------------------+
|   SplitService    |
+-------------------+
|                   |
+-------------------+
| +addExpense()     |
| +getUserBalance() |
| +settleBalance()  |
+-------------------+

+-------------------+
|   HistoryService  |
+-------------------+
|                   |
+-------------------+
| +getTransactionHistory()|
| +getGroupExpenses()     |
+-------------------+


Design Patterns used in this Splitwise design:

Strategy Pattern
Used for different split methods: Equal, Exact, Percentage.

Factory Pattern
Used to create the correct SplitStrategy based on split type.

Singleton Pattern
Used for SplitwiseService to have a single instance managing balances and transactions.

Observer Pattern (conceptually)
Can be used for notifying users when expenses are added or balances change.

*/