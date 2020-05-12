#include <iostream>

using namespace std;

class A
{
private:
    int a;
public:    
    A(): a{0}
    {
        // do nothing 
    }
    
    explicit A(const int val1): a{val1}
    {
        cout << "A created : " << a << endl;
    }
};

class B : public A
{
private:
    int b;
public:
    B(int val1, int val2) : A(val1), b{val2}
    {
        cout << "B created : " << b << endl;
    }
};


int main()
{
    B b(10, 20);
    cout << "End of main" << endl;
    
    return 0;
}