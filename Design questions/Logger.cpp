#include<iostream>
using namespace std;
class logger{
    public:
      logger* nxt;
      virtual void handler(string type,string msg)=0;
};

class info:public logger{
    public:
    
     void handler(string type,string msg)
     {
         if(type=="info")
         {
              cout<<"[info]"<<": "<<msg<<endl;
         }
         else{
              nxt->handler(type,msg);
         }
     }
};
class debug: public logger{
    public:
     void handler(string type,string msg)
     {
         if(type=="debug")
         {
              cout<<"[debug]"<<": "<<msg<<endl;
         }
         else{
              nxt->handler(type,msg);
         }
     }
};

class error: public logger{
    public:
     void handler(string type,string msg)
     {
         if(type=="error")
         {
              cout<<"[error]"<<": "<<msg<<endl;
         }
         else{
             cout<<"wrong type"<<endl;
         }
     }
};

class loggersystem{
    public:

    logger* chain;
    loggersystem()
    {
        logger* inf=new info();
        logger* er=new error();
        logger* db=new debug();
        inf->nxt=db;
        db->nxt=er;
        chain=inf;
    }
    void log(string type,string msg)
    {
         chain->handler(type,msg);
    }

};

int main()
{ 
    loggersystem* l=new loggersystem();
    l->log("info","hello system started");
    l->log("info","db has started");
    l->log("error","authentication eoor");

    return 1;
}