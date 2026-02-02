#include<iostream>
using namespace std;
class thirdparty{
    public :
    void paynow()
    {
        cout<<"payment is done by thirdparty"<<endl;
    }
};
class myapp{
    public:
    virtual void makepayment()=0;
};
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