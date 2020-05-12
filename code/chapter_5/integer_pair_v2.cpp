#include <iostream>
#include <iomanip>

using namespace std;

class Integer_Pair
{
protected:
    int i_;
    int j_;
public:
    // constructs an instance of Integer_Pair with provided values 
    Integer_Pair(int i = 0, int j = 0): i_{i}, j_{j}
    {
        cout << "An Object of Integer_Pair is instantiated with pair(" 
             << dec << i_ << ", " << j_ << ") " << endl;
    }
    
    int add() const
    {
        return i_ + j_;
    }
    
    int subtract() const 
    {
        return i_ - j_;
    }
    
    int multiply() const 
    {
        return i_ * j_;
    }
    
    double divide() const  
    {
        return ( static_cast<double>(i_)/static_cast<double>(j_) );
    }
    
    ~Integer_Pair()
    {
        cout << "Object - (" 
             << dec << i_ << ", " << j_ << ") " 
             << "has been destroyed!" << endl;
    }
};

class Integer_Pair_2 : public Integer_Pair
{
    // inheriting Base class constructor. 
    // No need of explicit constructor for derived class Integer_Pair_2
public:
    using Integer_Pair::Integer_Pair;
    void print()
    {
        cout << "(" << dec << Integer_Pair::i_ << ", " << Integer_Pair::j_ << ")" << endl;
    }
};

int main()
{
    Integer_Pair ip_1(1, 2);
    Integer_Pair ip_2(5, 5);
    
    // Now to show you how unique ip_1 and ip_2 are 
    cout << "ip_1's behavior : " << endl;
    cout << "ip1.add() gives " <<  ip_1.add() << endl;
    cout << "ip1.subtract() gives " <<  ip_1.subtract() << endl;
    cout << "ip1.multiply() gives " << ip_1.multiply() << endl;
    cout << "ip1.divide() gives "  << ip_1.divide() << endl;
    
    cout << "\nip_2's behavior : " << endl;
    cout << "ip2.add() gives " << ip_2.add() << endl;
    cout << "ip2.subtract() gives " << ip_2.subtract() << endl;
    cout << "ip2.multiply() gives " <<  ip_2.multiply() << endl;
    cout << "ip2.divide() gives "  << ip_2.divide() << endl;
    
    cout << "\nip_1 is stored @ " << hex << &ip_1 << endl;
    cout << "ip_2 is stored @ " << hex << &ip_2 << endl;
    
    // observe the trick to get the address of a class member function in C++ 
    int (Integer_Pair::*ptr_to_add)() const = &Integer_Pair::add;
    int (Integer_Pair::*ptr_to_multiply)() const = &Integer_Pair::multiply;
    
    cout << "address of add and multiply are as : "
         << hex  << (void*&)ptr_to_add 
         << ", " << (void*&)ptr_to_multiply
         << endl;
        
    cout << "\nsizeof(ip_1): " << dec << sizeof(ip_1) << endl;
    cout << "sizeof(ip_2): " << dec << sizeof(ip_2) << endl;
    
    cout << endl;
    
    Integer_Pair_2 ip_a_1(4,3);
    ip_a_1.print();
    
    cout << "sizeof(ip_a_1) : " << dec << sizeof(ip_a_1) << endl; 
    
    int (Integer_Pair::*ptr_to_add_parent)() const = &Integer_Pair::add;
    int (Integer_Pair::*ptr_to_add_derived)() const = &Integer_Pair_2::add;
    
    
    cout << "\naddress of add in parent class Integer_Pair: "
         << hex  << (void*&)ptr_to_add_parent 
         << "\naddress of add in derived class Integer_Pair_2: "
         << hex  << (void*&)ptr_to_add_derived << "\n"
         << endl;
    
    return 0;
    
}