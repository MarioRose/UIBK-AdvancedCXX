#include <iostream>

struct Vec2 {
    int x, y;
    Vec2(int x, int y) : x{x}, y{y} {}
    ~Vec2(){
        std::cout << "Vec2 destroyed\n";
    }
};

class ReferenceCount {
private:
    int refCount{0};
public:
    void increment(){
        refCount++;
    }

    void decrement(){
        refCount--;
    }

    int getCount(){
        return refCount;
    }
};



class unique_ptr_to_vec2 {

    Vec2* vec{nullptr};

public:
    unique_ptr_to_vec2() : vec{new Vec2{0,0}} {
        std::cout << "unique pointer created\n";
    }

    ~unique_ptr_to_vec2() {
        delete vec;
        vec = nullptr;
        std::cout << "unique pointer destroyed\n";
    }

    //copy assignment operator
    unique_ptr_to_vec2& operator= (const unique_ptr_to_vec2& rhs) = delete;
    //copy constructor
    unique_ptr_to_vec2(const unique_ptr_to_vec2& u) = delete;

    //move constructor
    unique_ptr_to_vec2(unique_ptr_to_vec2&& other) : vec{other.operator->()} {
        // other.operator->() = nullptr;
    }

    //move assignment operator
    unique_ptr_to_vec2& operator=(unique_ptr_to_vec2&& other) {
    	if(this != &other) {
    		vec = other.operator->();
    		return *this;
    	}
    }

    Vec2& operator * (){
        return *vec;
    }

    Vec2* operator->(){
        return vec;
    }

};

unique_ptr_to_vec2 generatePointer() {
    unique_ptr_to_vec2 ptr;
    return ptr;
}

class shared_ptr_to_vec2 {

    Vec2* vec{nullptr};
    ReferenceCount* counter{nullptr};

public:
    shared_ptr_to_vec2() : vec{new Vec2{0, 0}}, counter{new ReferenceCount()} {
        std::cout << "shared pointer created\n";
        counter->increment();
    }

    //Copy constructor
    shared_ptr_to_vec2(const shared_ptr_to_vec2& u) {
        vec = u.vec;
        counter = u.counter;
        counter->increment();
    }

    ~shared_ptr_to_vec2() {
        counter->decrement();
        if(counter->getCount() == 0){
            delete vec;
            delete counter;
            vec = nullptr;
            counter = nullptr;
            std::cout << "shared pointer destroyed\n";
        }
    }

    Vec2& operator * (){
        return *vec;
    }

    Vec2* operator->(){
        return vec;
    }

    //Copy assignment
    shared_ptr_to_vec2& operator= (const shared_ptr_to_vec2& rhs) {
        return *this;
    }


};

int main(int argc, char * argv[]){

    unique_ptr_to_vec2 testPointer;
    testPointer = generatePointer();
    shared_ptr_to_vec2 sharedTestPointer;

    shared_ptr_to_vec2 sharedTestPointer2 = sharedTestPointer;

    sharedTestPointer2->x = 3;

    {
        shared_ptr_to_vec2 sharedTestPointer3 = sharedTestPointer;

        std::cout <<  sharedTestPointer3.operator*().x << "\n";
    }

    std::cout <<  sharedTestPointer2.operator*().x << "\n";

    sharedTestPointer->x = 33;

    {
        unique_ptr_to_vec2 testPointer;
        std::cout <<  sharedTestPointer2.operator*().x << "\n";
    }

    std::cout << "Last Statement\n";
}