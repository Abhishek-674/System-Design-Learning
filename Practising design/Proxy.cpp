#include<iostream>
using namespace std;
class Database{
    public:
    void getaccess()
    {
        cout<<"take access"<<endl;
    }
};


class Proxy{
    public:
    Database* db;
    void getaccess(string role)
    {
         if(role=="admin")
         {
            db=new Database();
            db->getaccess();
         }
         else{
            cout<<"not access";
         }
    }
};
int main()
{
    Proxy* db=new Proxy();
    db->getaccess("admin");
    db->getaccess("asdf");

    return 1;
}