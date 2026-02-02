#include<iostream>
#include<vector>
using namespace std;
class Pizza{
    public:
    virtual int getPrice()=0;
};

class Magarita:public Pizza{
    public:
    int getPrice()
    {
        return 10;
    }
};

class Pepporoni:public Pizza{
    public:
     int getPrice()
     {
        return 20;
     }
};

class Decorator:public Pizza{
    public:
    Pizza* pizza;
    Decorator(Pizza* p){
        pizza=p;
    }
    virtual int getPrice()=0;
};

class chese:public Decorator{
    public:
    chese(Pizza* p1):Decorator(p1){};
    int getPrice()
    {
        return pizza->getPrice()+5;
    }
    
};
 class olive:public Decorator{
    public:
    olive(Pizza* p1):Decorator(p1){};
    int getPrice()
    {
        return pizza->getPrice()+8;
    }
 };

 class order{
    public:
    vector<Pizza*>v;
    void add(Pizza* p1)
    {
        v.push_back(p1);
    }
    int total()
    {
        int ans=0;
        for(auto it:v)
        {
            ans+=it->getPrice();
        }
        return ans;
    }
    void bill()
    {
        cout<<"your total bill is "<<total()<<endl;
    }

 };

int main()
{
    Pizza* p1=new Magarita();
    p1=new olive(p1);
    p1=new olive(p1);
    Pizza* p2=new Pepporoni();
    p2=new olive(p2);
    p2=new olive(p2);

    order* o1=new order();
    o1->add(p1);
    o1->add(p2);
    o1->bill();


    return 1;
}