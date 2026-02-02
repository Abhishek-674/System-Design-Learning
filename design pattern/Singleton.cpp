// Online C++ compiler to run C++ program online
#include<iostream>
using namespace std;

// using namespace std;
class Database{
    private:
    static Database* db;
    Database()
    {
        
    }
    public:
    static Database* getobj()
    {
        if(db==NULL)
        {
        //   cout<<"take object"<<endl;
           db= new Database(); 
           return db;
        }
        else
        {
            // cout<<"same object"<<endl;
            return db;
            
        }
        
    }
};
Database* Database::db=NULL;
int main() {
    Database* obj=Database::getobj();
    Database* obj2=Database::getobj();
    Database * obj3=Database::getobj();
    if(obj==obj2&&obj==obj3)
    {
        cout<<"same object";
    }
    else
    cout<<"diff rent object";

    return 0;
}