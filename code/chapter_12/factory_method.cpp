#include <iostream>
#include <memory>

using namespace std;

class shape;

enum class shape_type{circle, square};

class shape_factory
{
public:
	static shape* create_shape(shape_type type);
};

// shape is an abstract class
class shape
{
public:
	virtual ~shape()
	{
		// do nothing 
	}
	virtual void draw() const = 0; // pure virtual 
	// cannot create instances of shape, 
	// thus shape is an abstract base class
};

class circle : public shape
{
private:
	float radius_;
public:
	explicit circle(float radius = 0.0F) : radius_{radius}
	{
		// do nothing
	}
	~circle() 
	{ 
		cout << "circle with radius " << radius_ << " destroyed" << endl;
	}
	
	void draw() const override
	{
		cout << "Am a circle with radius " << radius_ << endl;
	}
};

class square : public shape
{
private:
	float length_;
public:
	explicit square(float length = 0.0F) : length_{length} {}
	~square()
	{
		cout << "square with length " << length_ << " destroyed" << endl;
	}
	void draw() const override
	{
		cout << "Am a square with length " << length_ << endl;
	}
};

shape* shape_factory::create_shape(shape_type type)
{
	switch(type)
	{
		case shape_type::circle:
		{
			float radius;
			
			cout << "Enter the radius for the new circle " << endl;
			cin >> radius;
			if(radius < 0.0F)
			{
				cout << "Bad radius entered. Taking default radius of 0.0F" << endl;
				radius = 0.0F;
			}
			
			return new circle(radius);
		}
		case shape_type::square:
		{
			float length;
			
			cout << "Enter the length for the new square " << endl;
			cin >> length;
			if(length < 0.0F)
			{
				cout << "Bad length entered. Taking default length of 0.0F" << endl;
				length = 0.0F;
			}
			
			return new square(length);
		}
	}// switch type ends here
}

void test_shape(shape_type type)
{
	// create a shape 
	// unique_ptr is very important
	unique_ptr<shape> shape_ptr(shape_factory::create_shape(type));
	// draw the shape
	shape_ptr->draw();
	
	return;
}

int main()
{
	// test circle
	test_shape(shape_type::circle);
	
	// test square
	test_shape(shape_type::square);
	
	cout << "Main ends here" << endl;
	
	return 0;
}