#include<iostream>
using namespace std;

class vehicle{
    public:
    string color;
    virtual vehicle* clone()=0;
    virtual void drive()=0;
};

class car:public vehicle{
    public:
    car(){}
    car(vehicle* v1)
    {
         this->color=v1->color;
    }
    void drive() override
    {
      cout<<"driving "<<color<<" car"<<endl;
    }

    vehicle* clone()
    {
         return new car(this);
    }
};
// class bike:public vehicle{
//     void drive()
//     {
//       cout<<"driving "<<color<<" bike"<<endl;
//     }

//     vehicle* clone()
//     {
//         return new bike();
//     }
// };
int main()
{
    vehicle* v1= new car();

    v1->color="red";
    v1->drive();
    vehicle* v2=v1->clone();
    v2->drive();

    v2->color="green";
    v2->drive();

    return 1;
}