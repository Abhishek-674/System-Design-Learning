#include<iostream>
#include<vector>
using namespace std;
class Filesystem{

    public:
    string name;
    int size;
    virtual void ls()=0;
    virtual int getsize()=0;
};

class File:public Filesystem{
    public:
    void ls()
    {
        cout<<this->name<<endl;
    }
    int getsize()
    {
        return this->size;
    }
};

class Folder:public Filesystem{
    public:
    vector<Filesystem*>fl;
    void ls()
    {
        cout<<this->name<<endl;
    }
    void lsall()
    {
        for(auto it:fl)
        {
            cout<<it->name<<endl;
        }
    }
    int getsize()
    {
        int total=0;
        for(auto it:fl)
        {
            total+=it->getsize();
        }
        return total;
    }
};

int main()
{
    return 1;
}