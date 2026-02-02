#include<iostream>
using namespace std;
class Vehicle {
    public:
    virtual void drive()=0;
    virtual Vehicle* clone()=0;
};
class car:public Vehicle{
    public:
    car(string c)
    {
        clr=c;
    }
    string clr="";
    car* clone() override
    {
        return new car(*this);
    }
    void drive()
    {
       cout<<"driving "<<clr<<" vehicle"<<endl;
    }
};
class bus:public Vehicle{
    public:
    bus(string c)
    {
        clr=c;
    }
    string clr="";
    bus* clone() override
    {
        return new bus(*this);
    }
    void drive()
    {
       cout<<"driving "<<clr<<" bus"<<endl;
    }
};
int main()
{ 
    Vehicle* v1=new car("red");
    Vehicle* v2=v1->clone();
    v1->drive();
    v2->drive();
     Vehicle* v3=new bus("red");
    Vehicle* v4=v3->clone();
    v3->drive();
    v4->drive();
    return 1;
}