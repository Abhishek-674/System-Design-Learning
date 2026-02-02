#include<iostream>
using namespace std;
class vehicle{
    public:
    virtual void drive()=0;
};

class car:public vehicle{
    public:
    void drive()
    {
        cout<<"driving car"<<endl;
    }
};
class nullvehicle:public vehicle{
    public:
    void drive()
    {
        cout<<"doing nothing"<<endl;
    }
};
class bike:public vehicle{
    public:
    void drive()
    {
        cout<<"driving"<<endl;
    }
};

class vehiclefactory{
    public:
    static vehicle* getobj(string type)
    {
        if(type=="car")
        return new car();
        if(type=="bike")
        return new bike();
        else 
        return new nullvehicle();
    }
};
int main()
{
    vehicle* v1=vehiclefactory::getobj("car");
    vehicle* v2=vehiclefactory::getobj("bike");
    vehicle* v3=vehiclefactory::getobj("null");
    v1->drive();
    v2->drive();
    v3->drive();
    return 1;
}