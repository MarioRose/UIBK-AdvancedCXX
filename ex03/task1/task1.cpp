#include <iostream>

struct Vec2 {
    int x, y;
    Vec2(double x, double y) : x(x), y(y) {}
    ~Vec2(){
        std::cout << "Vec2 destroyed\n";
    }
};

class unique_ptr_to_vec2 {

    Vec2* vec;

public:
    unique_ptr_to_vec2() {
        std::cout << "unique pointer created\n";
        vec = new Vec2(0, 0);
    }

    ~unique_ptr_to_vec2() {
        vec->~Vec2();
        std::cout << "unique pointer destroyed\n";
    }

    Vec2& operator * (){
        return *vec;
    }

    Vec2* operator->(){
        return vec;
    }

private:
    //Copy assignment
    unique_ptr_to_vec2& operator= (const unique_ptr_to_vec2& rhs) {}
    //Copy constuctor
    unique_ptr_to_vec2(const unique_ptr_to_vec2& u) {}

};

int main(int argc, char * argv[]){

    unique_ptr_to_vec2 testPointer;

    //unique_ptr_to_vec2 t = testPointer;
    

    {
        unique_ptr_to_vec2 testPointer;
    }

    std::cout << "Last Statement\n";
}