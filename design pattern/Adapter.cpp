#include<iostream>
using namespace std;

//Adaptee
class thirdparty{
    public :
    void paynow()
    {
        cout<<"payment is done by thirdparty"<<endl;
    }
};

// target
class myapp{
    public:
    virtual void makepayment()=0;
};

//Adapter
class adapter:public myapp{
    public:
    thirdparty* obj;
    adapter(thirdparty* obj)
    {
       obj=obj;
    }
 
    void makepayment() override{
        obj->paynow();
    }
};

int main()
{
    thirdparty* obj1=new thirdparty();
    myapp* app1=new adapter(obj1);
    app1->makepayment();
    return 0;
}

/*
Correct Adapter Pattern Relationships
1️⃣ Adapter implements / inherits Target

✔ YES

Adapter ─ ─ ─▷ Target   (implements / inherits interface)

2️⃣ Adapter has Adaptee

✔ YES

Adapter ◆────▶ Adaptee   (has-a / composition)

*/