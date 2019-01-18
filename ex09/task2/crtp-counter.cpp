#include <iostream>

template <typename T>
struct counter
{
    static int objects_created;
    static int objects_alive;

    counter()
    {
        ++objects_created;
        ++objects_alive;
    }

    counter(const counter&)
    {
        ++objects_created;
        ++objects_alive;
    }
protected:
    ~counter() // objects should never be removed through pointers of this type
    {
        --objects_alive;
    }
};

template <typename T> int counter<T>::objects_created( 0 );
template <typename T> int counter<T>::objects_alive( 0 );

class X : public counter<X> {};

class Y : public counter<Y> {};

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
