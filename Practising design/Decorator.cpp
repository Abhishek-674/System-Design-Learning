#include<iostream>
using namespace std;

class BasePizza{
    public:
    virtual int getprise()
    {
        return 50;
    }
};

class Decorator:public BasePizza{
    public:
    BasePizza* p;
    virtual int getprise()=0;
    

};

class cheese: public Decorator{
    public:
     cheese(BasePizza* pz)
    {
        this->p=pz;
    }
    int getprise()
    {
        return p->getprise()+5;
    }
};

class milk:public Decorator{
     public:
     milk(BasePizza* pz)
    {
        this->p=pz;
    }
    int getprise()
    {
        return p->getprise()+10;
    }
};
int main()
{
    BasePizza* pz=new BasePizza();
    cout<<pz->getprise()<<endl;

    pz=new cheese(pz);
    cout<<pz->getprise()<<endl;

    pz=new milk(pz);
    cout<<pz->getprise()<<endl;

    return 1;
}