#include<iostream>
#include<vector>
using namespace std;

class observer{
    public:
    virtual void updateme(string product)=0;
};
class user:public observer{
    string name;
     public:
     user(string name)
     {
        this->name=name;
     }
     void updateme(string product) override
     {
         cout<<"Hi "<<name<<" "<<product <<" product has came out of stock"<<endl;
     }
};

class subject{
    public:
    virtual void setsubscriber()=0;
    virtual void notifysubscriber()=0;
    virtual void updatestock()=0;
    
};

class product{
    int countProduct=0;
    
   
    public:
    string name;
     product(string name)
    {
        this->name=name;
    }
    vector<observer*>v;
    void setsubscriber(observer* obj)
    {
        v.push_back(obj);
    }
    void updatestock(int n)
    {
        countProduct=n;
        if(countProduct>0)
        {
            this->notifysubscriber();
        }
    }
    void notifysubscriber()
    {
        for(auto it:v)
        {
            it->updateme(name);
        }
    }


      
};
int main(){
    product* p1=new product("iphone");
     product* p2=new product("tata punch");
    observer* u1=new user("abhishek");
    observer* u2=new user("aditya");
    observer* u3=new user("ayushman");
    // observer* u1=new user("abhishek");
    p1->setsubscriber(u1);
    p1->setsubscriber(u2);
    p1->setsubscriber(u3);
    p2->setsubscriber(u1);
    p2->setsubscriber(u3);
    p2->updatestock(2);
    p1->updatestock(10);


    return 1;
}