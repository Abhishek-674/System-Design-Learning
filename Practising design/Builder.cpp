#include<iostream>
using namespace std;

class user{
    public:
    string name;
    string age;
    string email;
    public:
    user(string name,string age,string email)
    {
        this->name=name;
        this->age=age;
        this->email=email;
    }
    public:
    class builder{
        public:
        string name;
        string age;
        string email;
        builder setname(string name)
        {
             this->name=name;
             return *this;
        }
        builder setage(string age)
        {
            this->age=age;
            return *this;
        }
        builder setemail(string email)
        {
            this->email=email;
            return *this;
        }

        user* build()
        {
            return new user(name,age,email);
        }
    };
};


int main()
{
    user* u1=user::builder().setname("abhishek").setemail("abhishek@674").build();
    cout<<u1->name;
    return 1;
}
