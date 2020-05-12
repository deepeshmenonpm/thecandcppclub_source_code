#include <iostream>

using namespace std;

#ifdef DO_NOT_COMPILE
class A
{
private:
    int a;
    int b;
    int c;
public:    
    A(): a{0}, b{0}
    {
        // do nothing 
    }
    
    explicit A(const int val1)
    {
        a = 0; // this code is redundant as A() does this
        b = 0; // this code is redundant as A() does this
        c = val1;
        cout << "A created : " << a << ", " << b << ", " << c << endl;
    }
};

#endif

class A
{
private:
    int a;
    int b;
    int c;
public:    
    A(): a{0}, b{0}
    {
        // do nothing 
    }
    
    explicit A(const int val1) : A() //, c{val1} 'c' can be initialized like this also
    {
        c = val1;
        cout << "A created : " << a << ", " << b << ", " << c << endl;
    }
};

class B : public A
{
private:
    int b;
public:
    B(int val1, int val2) : A()
    {
        cout << "B created " << endl;
    }
};

void function(int value)
{
    cout << "value is " << value << endl;
}

void function(A a1)
{
    // do something with a1
    return;
}

int main()
{
    A a(20);
    
    cout << "End of main" << endl;
    
    return 0;
}