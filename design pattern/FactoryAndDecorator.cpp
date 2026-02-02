#include<iostream>
using namespace std;
class vehicle{
    // vehicle* nxt;
    public:
    virtual void drive(){};
    virtual int getprise()=0;
};
class car:public vehicle{
    void drive()
    {
        cout<<"driving a car"<<endl;
    }
    int getprise()
    {
        return 100;
    }
};

class bike:public vehicle{
    void drive()
    {
        cout<<"driving bike"<<endl;
    }
    int getprise()
    {
        return 50;
    }
};
class vehicleDecorator:public vehicle{
      public:
      vehicle* v;
      virtual int getprise()=0;
};

class sunroof:public vehicleDecorator{
    public:
    sunroof(vehicle* v)
    {
        this->v=v;
    }
    int getprise() override
    {
       return  v->getprise()+10;
    }
};

class extrafeature:public vehicleDecorator{
    public:
    extrafeature(vehicle* v)
    {
        this->v=v;
    }
    int getprise()
    {
        return v->getprise()+20;
    }
};


class Factory{
    public:
        static vehicle* getobj(string type)
        {
              if(type=="car")
              return new car();
              if(type=="bike")
              return new bike();
        }
};

int main()
{
    vehicle* v1=new car();
    v1=new sunroof(v1);
    v1=new extrafeature(v1);
    cout<<v1->getprise();
    return 1;
}