#include <iostream>

using namespace std;

// Stage 1: Write the template code
template<typename T>
T get_max( const T a, const T b )
{
    return ((a>=b)?a : b);
}

class Integer_Pair
{
private:
    int i_;
    int j_;
public:
    Integer_Pair(const int i = 0, const int j = 0): i_{i}, j_{j}
    {
        cout << "Integer_Pair (" << dec << i_ << ", " << j_ << ") created!" << endl;
    }
    
    bool operator>=(const Integer_Pair& other_integer_pair) const 
    {
        bool result = false;
        
        if(   (i_ >= other_integer_pair.i_) 
            &&(j_ >= other_integer_pair.j_) )
        {
            result = true;
        }
        
        return result;
    }
    
    friend ostream& operator<<(ostream& this_ostream, const Integer_Pair& a);
};

ostream& operator<<(ostream& this_ostream, const Integer_Pair& a)
{
    this_ostream << "(" << dec << a.i_ << ", " << a.j_ << ")";
    return this_ostream;
}

int main()
{
    int a, b;
    a = 5;
    b = 10;
    
    // stage 2: First usage with specified type as int
    cout << "max(5, 10) is " << dec << get_max<int>(a, b) << endl;
    
    // stage 2: First usage with specified type as float
    cout << "max(1.25F, -7.86F) is " << dec << get_max<float>(1.25F, -7.86F) << endl;
    
    Integer_Pair ip_1(1, 2);
    Integer_Pair ip_2(3, 4);
    
    cout << "max( " << ip_1 << ", " << ip_2 << " ) is " << get_max<Integer_Pair>(ip_1, ip_2) << endl;
    
    
    return 0;
}