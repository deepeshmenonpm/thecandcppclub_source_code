#include <iostream>
#include <memory>

using namespace std;

class Base
{
private:
    int a_;
public:
    Base(const int a = 0) : a_{a}
    {
        cout << "Base created with a_ : " << a_ << endl;
    }
    
    virtual ~Base()
    {
        cout << "Base object destroyed" << endl;
    }

protected: // MUST be protected so that this wont be available outside base/child classes 
    // copy constructor 
    Base(const Base& other_object)
    {
        a_ = other_object.a_;
        cout << "Base created with a_ : " << a_ << endl;
    }
    // Base class move constructor
    Base(Base&& other_object)
    {
        a_ = other_object.a_;
        other_object.a_ = 0;
        cout << "Base object has been move-constructed with a_ : " << a_ << endl;
    }
public:
    // copy assignment
    Base& operator=(const Base& other_object)
    {
        a_ = other_object.a_;
        cout << "Base object has been copy assigned with a_ : " << a_ << endl;
        return (*this);
    }
    
    // move assignment
    Base& operator=(Base&& other_object)
    {
        a_ = other_object.a_;
        other_object.a_ = 0;
        
        cout << "Base object has been move assigned with a_ : " << a_ << endl;
        
        return (*this);
    }
    
    virtual unique_ptr<Base> clone() 
    {
        cout << "Cloning Base object..." << endl;
        return make_unique<Base>(*this);
    }
    
    virtual unique_ptr<Base> move()
    {
        cout << "Moving Base object..." << endl;
        return make_unique<Base>(std::move(*this));
    }
    
    virtual void print()
    {
        cout << "a_ : " << a_ << endl; 
    }
    
    friend unique_ptr<Base> make_unique<Base>(Base&);
    friend unique_ptr<Base> make_unique<Base>(Base&&);
};

class Derived : public Base
{
private:
    int b_;
    int c_;
public:
    Derived(const int a = 0, const int b = 0, const int c = 0): Base(a), b_{b}, c_{c}
    {
        cout << "Derived created with b_ : " << b_ << " c_ : " << c_ << endl;
    }
    ~Derived()
    {
        cout << "Derived object destroyed" << endl;
    }
private:
    // copy constructor is made private 
    Derived(const Derived& other_object) : Base(other_object)
    {
        b_ = other_object.b_;
        c_ = other_object.c_;
        cout << "Derived created with b_ : " << b_ << " c_ : " << c_ << endl;
    }
    // move constructor
    Derived(Derived&& other_object): Base(std::move(other_object)) 
    {
        b_ = other_object.b_;
        c_ = other_object.c_;
        
        other_object.b_ = 0;
        other_object.c_ = 0;
        
        cout << "Derived object has been move-constructed with b_ : " << b_ 
             << " c_ : " << c_ << endl;
    }
public:
    // copy assignment
    Derived& operator=(const Derived& other_object)
    {
        // first assign base class object
        Base::operator=(static_cast<const Base&>(other_object)); 
        // no need to take the returned value as it is the same 'this' object
        
        b_ = other_object.b_;
        c_ = other_object.c_;
        cout << "Derived object has been copy assigned with b_ : " << b_ 
             << " c_ : " << c_ << endl;
             
        return (*this);
    }
    
    // move assignment
    Derived& operator=(Derived&& other_object)
    {
        // first assign base class object
        Base::operator=(static_cast<Base&&>(std::move(other_object))); 
        // no need to take the returned value as it is the same 'this' object
        
        b_ = other_object.b_;
        c_ = other_object.c_;
        
        other_object.b_ = 0;
        other_object.c_ = 0;
        
        cout << "Derived object has been move assigned with b_ : " << b_ 
             << " c_ : " << c_ << endl;
             
        return (*this);
    }

    unique_ptr<Base> clone() override
    {
        cout << "cloning Derived object..." << endl;
        return make_unique<Derived>(*this);
    }
    
    unique_ptr<Base> move() override
    {
        cout << "Moving Derived object..." << endl;
        return make_unique<Derived>(std::move(*this));
    }
    
    void print() override
    {
        // First print Base 
        Base::print();
        cout << "b_ : " << b_ << "\nc_ : " << c_ << endl;
    }
    
    friend unique_ptr<Derived> make_unique<Derived>(Derived&);
    friend unique_ptr<Derived> make_unique<Derived>(Derived&&);
};


void test_move_assignment()
{
    cout << "\nTesting move assignment..." << endl;
    
    Derived d5(10, 11, 12);
    
    Derived d6;
    
    d6 = std::move(d5);
    
    d6.print();
    
    cout << "Testing move assignment done\n" << endl;
    
    return;    
}


void test_copy_assignment()
{
    cout << "\nTesting copy assignment..." << endl;
    
    Derived d3(7, 8, 9);
    
    Derived d4;
    
    d4 = d3;
    
    d4.print();
    
    cout << "Testing copy assignment done\n" << endl;
    
    return;
}

void test_move_semantics()
{
    cout << "\nTesting move semantics..." << endl;
    
    Derived d2(4, 5, 6);
    
    unique_ptr<Base> b3 = d2.move();
    
    b3->print();
    
    cout << "Testing move semantics done!\n" << endl;
    
    return;
}

void test_clone()
{
    Derived d1(1, 2, 3);
    
    unique_ptr<Base> b1 = d1.clone();
    
    b1->print();    
    
    cout << "\nCreating a second clone of Derived..." << endl;
    unique_ptr<Base> b2 = b1->clone();
    
    b2->print();
    
    cout << "\nAt the end of test_clone() Object 'd1' in test_clone's stack,"
         << "\nalong with cloned objects will be destroyed!" << endl;
         
    return;
    
}

int main()
{
    test_clone();
    
    test_move_semantics();
    
    test_copy_assignment();
    
    test_move_assignment();    
    
    return 0;
}
