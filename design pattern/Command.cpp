#include <iostream>
using namespace std;

/* ---------- Receiver ---------- */
class Appliance {
public:
    virtual void turnon() = 0;
    virtual void turnoff() = 0;
    virtual ~Appliance() {}
};

class Ac : public Appliance {
public:
    void turnon() override {
        cout << "AC is ON" << endl;
    }
    void turnoff() override {
        cout << "AC is OFF" << endl;
    }
};

class Tv : public Appliance {
public:
    void turnon() override {
        cout << "TV is ON" << endl;
    }
    void turnoff() override {
        cout << "TV is OFF" << endl;
    }
};

/* ---------- Command Interface ---------- */
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};

/* ---------- Concrete Commands ---------- */
class Aconcommand : public Command {
    Appliance* app;
public:
    Aconcommand(Appliance* app) : app(app) {}
    void execute() override {
        app->turnon();
    }
};

class Acoffcommand : public Command {
    Appliance* app;
public:
    Acoffcommand(Appliance* app) : app(app) {}
    void execute() override {
        app->turnoff();
    }
};

class Tvoncommand : public Command {
    Appliance* app;
public:
    Tvoncommand(Appliance* app) : app(app) {}
    void execute() override {
        app->turnon();
    }
};

class Tvoffcommand : public Command {
    Appliance* app;
public:
    Tvoffcommand(Appliance* app) : app(app) {}
    void execute() override {
        app->turnoff();
    }
};

/* ---------- Invoker ---------- */
class Remote {
    Command* cmd;
public:
    void setcommand(Command* cmd) {
        this->cmd = cmd;
    }
    void pressButton() {
        if (cmd)
            cmd->execute();
    }
};

/* ---------- main (UNCHANGED) ---------- */
int main() {
    // Receivers
    Appliance* ac = new Ac();
    Appliance* tv = new Tv();

    // Commands
    Command* acOn = new Aconcommand(ac);
    Command* acOff = new Acoffcommand(ac);
    Command* tvOn = new Tvoncommand(tv);
    Command* tvOff = new Tvoffcommand(tv);

    // Invoker
    Remote remote;

    remote.setcommand(acOn);
    remote.pressButton();

    remote.setcommand(tvOn);
    remote.pressButton();

    remote.setcommand(acOff);
    remote.pressButton();

    remote.setcommand(tvOff);
    remote.pressButton();

    return 0;
}
