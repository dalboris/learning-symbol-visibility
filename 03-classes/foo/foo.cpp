#include "foo.h"

Foo::Foo(int a, int b) :
    a_(a),
    b_(b)
{

}

int Foo::add() const
{
    return a_ + b_;
}

int Foo::sub() const
{
    return a_ - b_;
}
