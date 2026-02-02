#include<iostream>
using namespace std;

class notedispenser{
    public:
      notedispenser* nxt;
      virtual void dispense(int amount)=0;
};

class note2000:public notedispenser{
    public:
    void dispense(int amount) override
    {
        if(amount>=2000)
        {
            cout<<"2000 note= "<<amount/2000<<endl;
       
              amount=amount%2000;
              if(amount==0)
              return ;
             
        }
        nxt->dispense(amount);
    }
};

class note1000:public notedispenser{
    public:
    void dispense(int amount) override
    {
        if(amount>=1000)
        {
            cout<<"1000 note= "<<amount/1000<<endl;
       
       
              amount=amount%1000;
              if(amount==0)
              return ;
              
        }
        nxt->dispense(amount);
    }
};

class note100:public notedispenser{
    public:
    void dispense(int amount) override
    {
        if(amount>=100)
        {
            cout<<"100 note= "<<amount/100<<endl;
        }
        else
        cout<<"enter amount greater than 100";
       
    }
};

class Atm{
    public:
    notedispenser* chain;
    Atm()
    {
        notedispenser* two=new note2000();
        notedispenser* one=new note1000();
        notedispenser* hun=new note100();

        two->nxt=one;
        one->nxt=hun;
        chain=two;

    }

    void withdraw(int amount)
    {
        if(amount%100!=0)
        cout<<"incorect amount enter multiple of 100"<<endl;
        else
        chain->dispense(amount);
    }
};
int main()
{
     Atm* atm=new Atm();
     atm->withdraw(1400);
     return 1;

}




/*


                    <<abstract>>
                +----------------------+
                |   CashDispenser      |
                +----------------------+
                | - next : CashDispenser*
                +----------------------+
                | + setNext(n)         |
                | + dispense(amount)  |
                +----------^-----------+
                           |
           -----------------------------------------
           |                  |                    |
+----------------+   +----------------+   +----------------+
| Dispenser2000  |   | Dispenser500   |   | Dispenser100   |
+----------------+   +----------------+   +----------------+
| + dispense()   |   | + dispense()   |   | + dispense()   |
+----------------+   +----------------+   +----------------+

                 +----------------------+
                 |        ATM           |
                 +----------------------+
                 | - chain : CashDispenser*
                 +----------------------+
                 | + withdraw(amount)  |
                 +----------------------+


Dispenser2000 IS-A CashDispenser
Dispenser500  IS-A CashDispenser
Dispenser100  IS-A CashDispenser


ATM HAS-A CashDispenser (has a means has an object of another class or uses another class)


Q1. Why did you choose Chain of Responsibility for ATM?

Answer:
Because cash dispensing can be broken into multiple independent handlers, each handling one denomination and forwarding the remaining amount.

Q2. What problem does CoR solve here?

Answer:
It removes complex if-else logic and allows easy addition of new denominations.

Q3. What is the client in this pattern?

Answer:
The ATM class.

Q4. Who creates the chain?

Answer:
ATM creates and links the cash dispenser chain.

🔹 Design & UML
Q5. Explain IS-A and HAS-A relationships in your design.

Answer:
Dispenser classes IS-A CashDispenser (inheritance).
ATM HAS-A CashDispenser (composition).
Each CashDispenser HAS-A reference to next handler (association).

Q6. Why did you use composition instead of aggregation?

Answer:
ATM fully owns the lifecycle of the cash dispenser chain.

Q7. Why is CashDispenser abstract?

Answer:
To enforce a common interface and enable polymorphism.

🔹 SOLID Principles
Q8. Which SOLID principles are used?

Answer:
Single Responsibility, Open–Closed, Liskov Substitution.

Q9. How does Open–Closed Principle apply?

Answer:
New denomination can be added without modifying existing code.

🔹 Edge Cases & Failure Handling
Q10. What if the amount is not divisible by 100?

Answer:
ATM rejects the request before starting the chain.

Q11. What if a denomination runs out of notes?

Answer:
That handler dispenses what it can and forwards the remaining amount.

Q12. What if the ATM cannot dispense the full amount?

Answer:
Transaction is aborted and account debit is rolled back.

🔹 Comparison Questions
Q13. Why not use Strategy pattern?

Answer:
Strategy selects one algorithm; CoR allows multiple handlers to process one request sequentially.

Q14. Difference between Chain of Responsibility and State?

Answer:
State changes behavior based on state;
CoR passes a request through multiple handlers.

🔹 Scalability & Extension
Q15. How to add ₹50 notes?

Answer:
Create Dispenser50 and attach it to the chain.

Q16. Can this work with multiple currencies?

Answer:
Yes, create separate chains per currency.

🔹 Performance & Safety
Q17. Time complexity?

Answer:
O(number of denominations), which is very small.

Q18. Is this thread-safe?

Answer:
ATM handles one transaction at a time; backend DB handles concurrency.

🔹 Final Wrap-Up (Very Common)
Q19. Explain your design in 30 seconds.

Answer:

“ATM uses Chain of Responsibility where each handler dispenses a specific denomination. ATM owns the chain and sends the withdrawal request to it. This design is extensible, avoids conditional logic, and follows SOLID principles.”

Q20. What would you improve if given more time?

Answer:
Add State pattern, logging, monitoring, and note availability tracking.



*/