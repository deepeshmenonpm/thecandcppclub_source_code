#include <iostream>
#include <string>


using namespace std;

enum class light_technology{incandescent = 0, fluorescent};

class Light_Device
{
public:
    ~Light_Device(){}
    virtual void on() =0;
    virtual void off() =0;
    virtual void change_brightness() =0;
};

class Incandescent_Device : public Light_Device
{
private:
    float current_brightness;
public:
    Incandescent_Device() : current_brightness{0.0F}
    {}
    ~Incandescent_Device(){}
    void on()
    {
        current_brightness = 100.0F;
        cout << "Incandescent bulb turned on" << endl;
    }
    
    void off()
    {
        current_brightness = 0.0F;
        cout << "Incandescent bulb turned off" << endl;
    }
    
    void change_brightness()
    {
        cout << "Enter the required brightness level" << endl;
        float percent;
        cin >> percent;
        if((percent < 0.0F) || (percent > 100.0F))
        {
            cout << "Invalid brightness entered" << endl;
        }
        else 
        {
            current_brightness = percent;
            cout << "Brightness set at " << current_brightness << endl; 
        }
        
    }
    
};

class Fluorescent_Device : public Light_Device
{
public:
    Fluorescent_Device(){}
    ~Fluorescent_Device(){}
    void on()
    {
        cout << "Fluorescent bulb turned on" << endl;
    }
    
    void off()
    {
        cout << "Fluorescent bulb turned off" << endl;
    }
    
    void change_brightness()
    {
        cout << "This operation not supported for fluorescent device" << endl;        
    }
    
};

class Bulb;

class State
{
public:
    State(){}
    virtual ~State(){}
    virtual void on(Bulb *bulb);
    virtual void off(Bulb *bulb);
    virtual void change_brightness(Bulb *bulb);
};

class Bulb
{
private:
    State *current;
    Light_Device *light_device;
public:
    Bulb(light_technology technology, State *state) : current{state}
    {
        switch(technology)
        {
            case light_technology::fluorescent:
            {
                light_device = new Fluorescent_Device();
                break;
            }
            case light_technology::incandescent:
            default:
            {
                light_device = new Incandescent_Device();
                break;
            }            
        }
    }
    ~Bulb()
    {
        delete light_device;
    }
    
    void set_current_state(State *state)
    {
        current = state;
        return;
    }
    
    void on()
    {
        current->on(this);
        return;
    }
    
    void off()
    {
        current->off(this);
        return;
    }
    
    void change_brightness()
    {    
        current->change_brightness(this);
        return;
    }
    
    Light_Device* get_light_device()
    {
        return light_device;
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

void State::change_brightness(Bulb *bulb)
{
    cout << "Not supported in off state" << endl;
    return;
}


class On_State : public State
{
public:
    On_State(){}
    ~On_State(){}
    
    void on(Bulb *bulb);
    void off(Bulb *bulb);
    void change_brightness(Bulb *bulb);
};

class Off_State : public State
{
public:
    Off_State(){}
    ~Off_State(){}
    
    void on(Bulb *bulb);
    void off(Bulb *bulb);
    void change_brightness(Bulb *bulb);
};

void On_State::on(Bulb *bulb)
{
    // nothing to implement
    // just delegate this to base class 
    State::on(bulb);
}

void On_State::change_brightness(Bulb *bulb)
{
    auto light_device = bulb->get_light_device();
    light_device->change_brightness();
}

void On_State::off(Bulb *bulb)
{
    // create an Off_State instance
    Off_State *new_state = new Off_State();
    bulb->set_current_state(new_state);
    auto light_device = bulb->get_light_device();
    light_device->off();
    
    delete this;
}

void Off_State::on(Bulb *bulb)
{    
    // create an Off_State instance
    On_State *new_state = new On_State();
    bulb->set_current_state(new_state);
    auto light_device = bulb->get_light_device();
    light_device->on();
    
    delete this;
}

void Off_State::off(Bulb *bulb)
{
    // nothing to implement
    // just delegate this to base class 
    State::off(bulb);
    
    return;
}

void Off_State::change_brightness(Bulb *bulb)
{
    // nothing to do 
    // just delegate to base class 
    State::change_brightness(bulb);
}


int main()
{
    string choice;
    string technology;
    Bulb *b1;
    
    cout << "Bulb controller application" << endl;
    cout << "Enter the bulb technology required :\n(I) : Incandescent\n(F) : Fluorescent" << endl;
    
    cin >> technology;
    
    if(0 == technology.compare("F"))
    {
        b1 = new Bulb(light_technology::fluorescent, new Off_State());
    }
    else
    {
        b1 = new Bulb(light_technology::incandescent, new Off_State());
    }
    
    
    while(true)
    {
        choice.clear();
        cout << "What to do next : ON, OFF, SET_BRIGHTNESS, EXIT" << endl;
        cin >> choice;
        
        if(0 == choice.compare("ON"))
        {
            b1->on();
        }
        else if(0 == choice.compare("OFF"))
        {
            b1->off();
        }
        else if(0 == choice.compare("SET_BRIGHTNESS"))
        {
            b1->change_brightness();
        }
        else if(0 == choice.compare("EXIT"))
        {
            cout << "Exiting bulb controller application" << endl;
            break;
        }
    }
    
    if(b1 != nullptr)
    {
        delete b1;
    }
    
    return 0;
}
