#include <iostream>
using namespace std;

class Traffic;

// STATE INTERFACE
class state {
public:
    virtual void change(Traffic* t) = 0;
    virtual ~state() {}
};

// CONTEXT
class Traffic {
public:
    state* st;

    void setstate(state* s) {
        st = s;
    }

    void change() {
        st->change(this);
    }
};

// STATES
class red : public state {
public:
    void change(Traffic* t);
};

class yellow : public state {
public:
    void change(Traffic* t);
};

class green : public state {
public:
    void change(Traffic* t);
};

// ---- Method definitions AFTER full class definitions ----

void red::change(Traffic* t) {
    cout << "red -> stop" << endl;
    t->setstate(new yellow());
}

void yellow::change(Traffic* t) {
    cout << "yellow -> ready" << endl;
    t->setstate(new green());
}

void green::change(Traffic* t) {
    cout << "green -> go" << endl;
    t->setstate(new red());
}

int main() {
    Traffic* t = new Traffic();
    t->setstate(new red());
    t->change();

    return 0;
}
