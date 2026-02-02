#include<iostream>
using namespace std;

class singleton{
    static singleton* obj;
    string name;
    singleton()
    {

    }
    public:
    static singleton* getinstance()
    {
        if(obj==NULL)
        {
            obj=new singleton();
        }
        return obj;
    }

};
singleton* singleton::obj=nullptr;
int main()
{
    singleton* obj=singleton::getinstance();
    singleton* obj2=singleton::getinstance();
     singleton* obj3=singleton::getinstance();
    if(obj==obj2&&obj==obj3)
    cout<<"same";
    else
    cout<<"not same"<<endl;

}