#include<iostream>
using namespace std;

class LoggingSystem{
    public:
    LoggingSystem* nxt;
    virtual void handle(string type)=0;
};

class info:public LoggingSystem{
    public:
    void handle(string type) override
    {
        if(type=="info")
        {
            cout<<"starting system"<<endl;
        }
        else{
            nxt->handle(type);
        }
    }
};
class logger:public LoggingSystem{
    public:
    void handle(string type) override
    {
        if(type=="log")
        {
            cout<<"logging system"<<endl;
        }
        else{
            nxt->handle(type);
        }
    }
};

class error:public LoggingSystem{
    public:
    void handle(string type) override
    {
        if(type=="error")
        {
            cout<<"error system"<<endl;
        }
        else{
            
        }
    }
};

class chaining{
    public:
    LoggingSystem* ch;
    chaining()
    {
        LoggingSystem* in=new info();
        LoggingSystem* lg=new logger();
        LoggingSystem* er=new error();
        in->nxt=lg;
        lg->nxt=er;
        ch=in;
    }
    void handle(string type)
    {
        ch->handle( type);
    }

};

// class 

int main()
{
    chaining* ch=new chaining();
    ch->handle("error");
    return 1;
}