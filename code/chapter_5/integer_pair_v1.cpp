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
		cout << "An Object of Integer_Pair is inistantiated with pair(" 
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
	
	virtual double divide() const  
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

class Swapped_Pair : public Integer_Pair
{
	
public:
	using Integer_Pair::Integer_Pair;
	//using Integer_Pair2::Integer_Pair;
	double divide() const override
	{
		return (  static_cast<double>(j_) \
		         /static_cast<double>(i_) );
	}
};

// ip_obejct is a reference to an object of the type Inetger_Pair 
void integer_pair_client(Integer_Pair &ip_object)
{
	// add is not a virtual function so this invokes base class add()
	cout << "\nip_object.add() : " << dec << ip_object.add() << endl;
	// divide is polymorphic, so this invokes derived calss divide 
	cout << "ip_object.divide() : " << ip_object.divide() << endl;
	
	return;
}

int main()
{
	Integer_Pair ip_1(1, 2);
	Swapped_Pair ip_b_1(1,2);
	
	cout << "\nip_b_1.divide : " << ip_b_1.divide() << endl;
	cout << "ip_1.divide : " << ip_1.divide() << endl;
	
	cout << "\nsizeof(ip_1): " << dec << sizeof(ip_1) << endl;
	cout << "sizeof(ip_b_1): " << dec << sizeof(ip_b_1) << endl;
	
	//Code to the base class principle 	
	integer_pair_client(ip_b_1);
	
	cout << endl;
	
	return 0;
	
}