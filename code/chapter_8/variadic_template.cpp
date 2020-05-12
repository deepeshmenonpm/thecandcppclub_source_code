#include <iostream>

using namespace std;

// This function is to stop recursive function calls 
// Finally the arglist will come to an end, i.e. no more arguments
// or, just empty arguments
void printf()
{
    cout << endl;
}

template<typename T, typename... Arglist>
void printf(T head, Arglist... tail)
{
    cout << head;
    // recursively call printf with the rest of the arguments
    printf(tail...);
    
    return;
}

int main()
{
    printf("A Set : {", 1, ", ", 2, ", ", 3, "}");
    
    return 0;
}