#include<iostream>
#include<vector>
#include<map>
using namespace std;

class Publisher{
     public:
     string msg;
    //  Topic* tp;
    Broker* b;
     void createmsg(string msg)
     {
         this->msg=msg;
         b->sendmsg(msg,"sms");
     }
};

class Topic{
    public:
    string msg;
    vector<Subscriber*>subs;
    void notify(string msg)
    {
         for(auto it:subs)
         {
            it->getmsg(msg);
         }
    }
    void addsubscriber(Subscriber* s)
    {
        subs.push_back(s);
    }


};

class Broker{
    public:
    map<string,Topic*>tp;
    void createtopic(string topic)
    {
        tp[topic]=new Topic(); 
    }
    void addsubscriber( string topic,Subscriber* s)
    {
       tp[topic]->addsubscriber(s);
    }
    void sendmsg(string msg,string topic)
    {
        
             tp[topic]->notify(msg);
         
    }

};

class Subscriber{
     public:
      void getmsg(string msg)
      {
          cout<<"you have got a msg "<<msg<<endl;
      }
};
int main()
{
    return 1;
}

/*
Publisher.publish()
   ↓
Broker.publish()
   ↓
Topic.notifySubscribers()
   ↓
Subscriber.receive()
+--------------------------------------------------+
|                   Publisher                      |
+--------------------------------------------------+
| - broker : Broker*                               |
+--------------------------------------------------+
| + publish(msg : string, topic : string) : void   |
+--------------------------------------------------+
                 |
                 | uses
                 v
+--------------------------------------------------+
|                     Broker                       |
+--------------------------------------------------+
| - topics : map<string, Topic*>                   |
+--------------------------------------------------+
| + createTopic(name : string) : void              |
| + subscribe(topic : string, s : Subscriber*)    |
| + publish(topic : string, msg : string) : void   |
+--------------------------------------------------+
                 |
                 | owns
                 v
+--------------------------------------------------+
|                      Topic                       |
+--------------------------------------------------+
| - name : string                                  |
| - subscribers : vector<Subscriber*>              |
+--------------------------------------------------+
| + addSubscriber(s : Subscriber*) : void          |
| + notify(msg : string) : void                    |
+--------------------------------------------------+
                 |
                 | notifies (1 → many)
                 v
+--------------------------------------------------+
|                   Subscriber                     |
+--------------------------------------------------+
| + onMessage(msg : string) : void                 |
+--------------------------------------------------+

*/