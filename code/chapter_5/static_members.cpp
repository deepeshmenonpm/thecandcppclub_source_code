#include <iostream>
#include <string>

using namespace std;

class Student
{
protected:
    static int count; // static is just for delcaring count.
    static const float record_version; // or use constexpr instead const for in-class initialization
    string name_;
    int marks_;

public:
    Student(const char *name, const int marks = 0): name_{name}, marks_{marks}
    {
        ++count;
        cout << "Student Record #" << dec << count << " for - " << name_ 
             << " with marks " << marks_ << " is created!" 
             << endl;
    }
    
    void set_marks(const int new_marks)
    {
        marks_ = new_marks;
    }
    
    void print_record(void) const // note the parameter list is marked as void
    {
        cout << "Student name - "  << this->name_ // this pointer still does its job :) 
             << " marks - " << dec << this->marks_ 
             << endl;
        
        return;
    }
    
    // static member function 
    static int get_current_count()
    {
        return count;
    }
    
    static float get_record_version()  
    {
        return record_version;
    }
    
    // overloaded version of set_marks function 
    static int set_marks(Student *this_student, const int marks)
    {
        this_student->marks_ = marks;
        cout << "Student - " << this_student->name_ 
             << " marks is updated to " << this_student->marks_
             << endl;
    }
    
    ~Student()  
    {
        --count;
    }    
};

// important step to instantiate and initialize static data of class 
int Student::count = 0; // note: dont add static keyword again here
const float Student::record_version = 1.2F;

class Student_Extended : public Student
{
public:
    enum class medium{english = 0, native = 1};
    enum class syllabus{center = 0, international = 1};
private:
    int standard_;
    medium med_;
    syllabus syl_;
    static const float record_version;
public:
    Student_Extended( const char *const name,
                      const int standard = 1, // default value is 1
                      const medium med = Student_Extended::medium::english, // default medium english
                      const syllabus syl = Student_Extended::syllabus::center ): // default syllabus center
        Student(name),    // calling base class constructor
        standard_{standard}, med_{med}, syl_{syl}// members are initialized
    {
        cout << "Student Record created with the details: \n"
             << "standard - " << dec << standard_  
             << ", in medium - " << med_ 
             << ", affiliated to " << syl_ << " syllabus"
             << endl;
    }
    
    static int get_current_count() 
    {
        return Student::count;
    }
    
    static float get_own_record_version()
    {
        return Student_Extended::record_version;
    }
    
    static float get_student_record_version()
    {
        return  Student::record_version;
    }
    
    // operator << has to be overloaded for enum medium and enum syllabus
    // need to use friend functions semantics here as << has to work on private class members 
    friend ostream& operator<<(ostream& this_ostream, const Student_Extended::medium& this_medium);
    friend ostream& operator<<(ostream& this_ostream, const Student_Extended::syllabus& this_syllabus);
    
};

const float Student_Extended::record_version = 5.7F;

ostream& operator << (ostream& this_ostream, const Student_Extended::medium& this_medium)
{
    switch(this_medium)
    {
        case (Student_Extended::medium::english): 
        {
            this_ostream << " English";
            break;
        }
        default:
        {
            this_ostream << " native";
            break;
        }
    }
    
    return this_ostream;
}

ostream& operator << (ostream& this_ostream, const Student_Extended::syllabus& this_syllabus)
{
    switch(this_syllabus)
    {
        case (Student_Extended::syllabus::center): 
        {
            this_ostream << " Center";
            break;
        }
        default:
        {
            this_ostream << " International";
            break;
        }
    }
    
    return this_ostream;
}

int main()
{
    Student s1("Student_1", 95);
    s1.print_record();
    
    // See the magic of using a static member function to change student s1's marks
    Student::set_marks(&s1, 97);
    
    cout << "sizeof(s1) : " << dec << sizeof(s1) << endl;
    cout << "sizeof(Student) : " << dec << sizeof(Student) << endl;
    
    Student_Extended s2("Student_2"); // take other parameters as defaults 
    cout << "Number of student record(s) created : " << s2.get_current_count() << endl;
    
    cout << "Student Record Version : " << Student::get_record_version() << endl;
    cout << "Student Extended Record Version : " << Student_Extended::get_own_record_version() << endl;
    cout << "Student Base Record Version : " << Student_Extended::get_student_record_version() << endl;
    
    return 0;
} 

