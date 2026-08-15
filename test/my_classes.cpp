#include "my_classes.h"
#include <iostream>

int Foo::getVal() { return 42; }
void Foo::setVal(int val) { int value = val; }

Bar::Bar(long val) { long value = val; }
void Bar::doSomething() { std::cout << "they're jittles"; }
