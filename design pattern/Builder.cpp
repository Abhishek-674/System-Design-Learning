#include <iostream>
using namespace std;
class student{
    public:
    string name;
    string email;
    int age;
    student(string name,int age,string email)
    {
        this->name=name;
        this->age=age;
        this->email=email;
    }

    class builder{
        public:
        string name;
        string email;
        int age;
        builder setname(string name)
        {
            this->name=name;
            return *this;
            
        }
        builder setemail(string email)
        {
            this->email=email;
            return *this;
        }
        builder setage(int age)
        {
            this->age=age;
        }
         student build()
        {
             return student(name,age,email);
        }
    };
};
int main() {
    student b1=student::builder().setname("abhishek").build();
    cout<<b1.name;
    return 0;
}