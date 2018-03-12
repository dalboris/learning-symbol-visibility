#include <iostream>
#include "foo.h"

int main()
{
    int a = 5;
    int b = 3;
    Foo f(a, b);

    std::cout << "a + b = " << f.add() << std::endl;
    std::cout << "a - b = " << f.sub() << std::endl;

    return 0;
}
