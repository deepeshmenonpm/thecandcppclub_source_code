#include <iostream>
#include <string>


using namespace std;

class Bulb;

class State
{
public:
    State(){}
    virtual ~State(){}
    virtual void on(Bulb *bulb);
    virtual void off(Bulb *bulb);
};

class Bulb
{
private:
    State *current;
public:
    Bulb(State *state) : current{state} {}
    ~Bulb(){}
    
    void set_current_state(State *state)
    {
        current = state;
    }
    
    void on()
    {
        current->on(this);
    }
    
    void off()
    {
        current->off(this);
    }
    
};


void State::on(Bulb *bulb)
{
    cout << "already in on state" << endl;
    return;
}

void State::off(Bulb *bulb)
{
    cout << "already in off state" << endl;
    return;
}


class On_State : public State
{
public:
    On_State(){}
    ~On_State(){}
    
    void on(Bulb *bulb);
    void off(Bulb *bulb);
};

class Off_State : public State
{
public:
    Off_State(){}
    ~Off_State(){}
    
    void on(Bulb *bulb);
    void off(Bulb *bulb);
};

void On_State::on(Bulb *bulb)
{
    // nothing to implement
    // just delegate this to base class 
    State::on(bulb);
}

void On_State::off(Bulb *bulb)
{
    // create an Off_State instance
    Off_State *new_state = new Off_State();
    bulb->set_current_state(new_state);
    cout << "Bulb is switched off" << endl;
    
    delete this;
}

void Off_State::on(Bulb *bulb)
{    
    // create an Off_State instance
    On_State *new_state = new On_State();
    bulb->set_current_state(new_state);
    cout << "Bulb is switched on" << endl;
    
    delete this;
}

void Off_State::off(Bulb *bulb)
{
    // nothing to implement
    // just delegate this to base class 
    State::off(bulb);
    
    return;
}

int main()
{
    string choice;
    
    Bulb b1(new Off_State());
    
    cout << "Bulb controller application" << endl;
    
    while(true)
    {
        choice.clear();
        cout << "What to do next : ON, OFF, EXIT" << endl;
        cin >> choice;
        
        if(0 == choice.compare("ON"))
        {
            b1.on();
        }
        else if(0 == choice.compare("OFF"))
        {
            b1.off();
        }
        else if(0 == choice.compare("EXIT"))
        {
            cout << "Exiting bulb controller application" << endl;
            break;
        }
    }
    
    return 0;
}




