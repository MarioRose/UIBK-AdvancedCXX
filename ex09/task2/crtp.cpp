// https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c

#include <iostream>
using namespace std;

template <typename Child>
struct Base
{
    void interface()
    {
        static_cast<Child*>(this)->implementation();
    }
};

struct Derived : Base<Derived>
{
    void implementation()
    {
        cerr << "Derived implementation\n";
    }
};

int main()
{
    Derived d;
    d.interface();  // Prints "Derived implementation"
}
