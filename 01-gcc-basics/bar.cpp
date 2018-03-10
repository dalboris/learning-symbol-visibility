#include <iostream>
#include "foo.h"

int main()
{
    int a = 5;
    int b = 3;

    // Can't use add(a, b) and mult(a, b): they're not part of foo's API! :(
    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << sub(a, b) << std::endl;
    std::cout << "a * b = " << a * b << std::endl;

    return 0;
}
