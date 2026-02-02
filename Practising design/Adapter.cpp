// #include<iostream>
// using namespace std;
// // 
// class Upi{
//     public:
//     void payment()
//     {
//         cout<<"payment is done by upi"<<endl;
//     }
// };


// class paymentmethod{
//     public:
//     virtual void makepayment()=0;
// };

// class pay:public paymentmethod{
//     public:
//     Upi* md;
//     pay(Upi* md)
//     {
//          this->md=md;
//     }
//     void makepayment()
//     {
//          md->payment();
//     }
// };
// int main()
// {
//     Upi* upi=new Upi();
//     paymentmethod* p=new pay(upi);
//     p->makepayment();
//     // p->makepayment(m2);

//     return 1;
// }



#include<iostream>
using namespace std;

class Upi{
    public:
    void pay()
    {
        cout<<"paying by upi "<<endl;
    }
};

class Paym{
    public:
    virtual void paynow()=0;
};

class payment:public Paym{
    public:
    Upi* up;
    payment(){};
    payment(Upi* up)
    {
       this->up=up;
    }
    void paynow() override
    {
        up->pay();

    }

};


int main()
{
    Upi* u=new Upi(); 
    Paym* p=new payment(u);
    p->paynow();
    return 1;

}