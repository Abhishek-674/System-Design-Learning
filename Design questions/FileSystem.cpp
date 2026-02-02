#include<iostream>
#include<vector>
using namespace std;
class Filesystem{
    public:
    int size;
    string name;
    virtual void ls()=0;
    virtual int getsize()=0;
    virtual void add(Filesystem* f)
    {

    }
};

class File:public Filesystem{
    public:
    File(string name,int size)
    {
        this->name=name;
        this->size=size;
    }
    int getsize() override
    {
        return this->size;
    }
    void ls() override
    {
        cout<<this->name<<endl;
    }
};

class Folder:public Filesystem{
    public:
    Folder(string name)
    {
        this->name=name;
        this->size=0;
    }
    vector<Filesystem*>fl;
    void ls() override{
         cout<<this->name<<"/"<<endl;
         for(auto it:fl)
         {
            cout<<it->name<<endl;
         }
    }
    void add(Filesystem* f)
    {
         fl.push_back(f);
    }

    int getsize() override
    {
        int total=0;
        for(auto it:fl)
        {
            total+=it->getsize();
        }
        cout<<total<<endl;
        return total;
    }

};

int main()
{
     Filesystem* folder1=new Folder("desktop");
     Filesystem* folder2=new Folder("DOCUMENT");
      Filesystem* folder3=new Folder("COLLEGE");
       Filesystem* f1=new File("abc.txt",10);
        Filesystem* f2=new File("pan.txt",20);
         Filesystem* f3=new File("adhar.txt",2);
          Filesystem* f4=new File("code.txt",4);
          folder1->add(folder2);
          folder1->add(folder3);
          folder1->add(f1);
          folder2->add(f2);
          folder3->add(f4);
          folder2->add(f3);

          folder1->ls();



}
/*

                +----------------------------+
                |        Filesystem          |   <<Abstract>>
                +----------------------------+
                | - name : string            |
                | - size : int               |
                +----------------------------+
                | + ls() : void              |
                | + lsRecursive() : void     |
                | + getSize() : int           |
                | + add(f: Filesystem*)      |
                +----------------------------+
                          ▲
                          |
          -----------------------------------------
          |                                       |
+-----------------------+           +-----------------------+
|         File          |           |        Folder         |
+-----------------------+           +-----------------------+
|                       |           | - children : vector   |
+-----------------------+           +-----------------------+
| + ls()                |           | + add(f)              |
| + lsRecursive()       |           | + ls()                |
| + getSize()           |           | + lsRecursive()       |
+-----------------------+           | + getSize()           |
                                    +-----------------------+
                                              |
                                              | has
                                              v
                                   +-----------------------+
                                   |      Filesystem       |
                                   +-----------------------+


IS-A (Inheritance)

File IS-A Filesystem
Folder IS-A Filesystem

HAS-A (Composition)

Folder HAS-A collection of Filesystem
*/