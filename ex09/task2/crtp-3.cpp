#include <iostream>

struct Counter
{
    static int objects_created;
    static int objects_alive;

    Counter()
    {
        ++objects_created;
        ++objects_alive;
    }

    Counter(const Counter&)
    {
        ++objects_created;
        ++objects_alive;
    }
protected:
    ~Counter() // objects should never be removed through pointers of this type
    {
        --objects_alive;
    }
};

int Counter::objects_created( 0 );
int Counter::objects_alive( 0 );

class X : public Counter {};

class Y : public Counter {};

int main() {

    {
        X x1;
        X x2(x1);
    }

    X x3;
    Y y1;

    std::cout << "X alive: " << X::objects_alive << ", created: " << X::objects_created << std::endl;
    std::cout << "Y alive: " << Y::objects_alive << ", created: " << Y::objects_created << std::endl;

}
