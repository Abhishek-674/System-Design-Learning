#include<iostream>
#include<vector>
using namespace std;
class Filesystem{
    public:
    virtual int getsize()=0;
    virtual void ls()=0;
    virtual string getname()=0;
};

class File:public Filesystem{
    string name;
    int size=0;
    public:
    File(string name,int n)
    {
        this->name=name;
        size=n;
    }
    void ls()override{
          cout<<name<<endl;
    } 
    int getsize()override
    {
        return size;
    }
    string getname()override
    {
        return this->name;
    }

};
class Folder:public Filesystem{
    string name;
    vector<Filesystem*>v;
    public:
    Folder(string name)
    {
       this->name=name;
    }
    void push(Filesystem* f)
    {
        v.push_back(f);
    }
    string getname()
    {
        return this->name;
    }
    void ls()override
    {
         cout<<name<<"/"<<endl;
        for(auto it:v)
        {
           cout<<it->getname()<<endl;
        }
    }
    int getsize()override
    {
        int ans=0;
        for(auto it:v)
        {
            ans+=it->getsize();
        }
        return ans;
    }
};
int main()
{
    Folder* f1=new Folder("dektop");
    File* f2=new File("code.cpp",5);
    File* f3=new File("abc.txt",20);
    Folder* f4=new Folder("school");
    File* f5=new File("abc.txt",3);
    f1->push(f2);
    f1->push(f3);
   f1->push(f4);
   f4->push(f5);

   f4->ls();


    return 1;
}


/*
Key Participants
Client
Component (interface / abstract class)
Leaf
Composite


        Client
          |
          v
      <<interface>>
        Component
        + operation()

    is-a     ▲.      is-a
     ┌───────┴────────┐
     |                |
   Leaf           Composite
    (file)           ◆ (folder)
                    | has a
               Component (children)
Leaf is a 
composite is a and has a (both relation exits)

*/