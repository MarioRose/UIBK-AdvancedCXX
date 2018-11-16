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
    }

    //move assignment operator
    unique_ptr_to_vec2& operator=(unique_ptr_to_vec2&& other) {
    	if(this != &other) {
            delete vec;
            vec = nullptr;

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


class shared_ptr_to_vec2 {

    Vec2* vec{nullptr};
    ReferenceCount* counter{nullptr};

public:
    shared_ptr_to_vec2() : vec{new Vec2{0, 0}}, counter{new ReferenceCount()} {
        std::cout << "shared pointer created\n";
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

    //Copy constructor
    shared_ptr_to_vec2(const shared_ptr_to_vec2& u) {
        vec = u.vec;
        counter = u.counter;
        counter->increment();
    }

    //Copy assignment
    shared_ptr_to_vec2& operator= (const shared_ptr_to_vec2& rhs) {
        return *this;
    }

    //move constructor
    shared_ptr_to_vec2(shared_ptr_to_vec2&& other) : vec{other.operator->()}, counter{other.getCounterPointer()} {
        counter->increment();
    }

    //move assignment operator
    shared_ptr_to_vec2& operator=(shared_ptr_to_vec2&& other) {
        if(this != &other) {
            delete vec;
            delete counter;
            vec = nullptr;
            counter = nullptr;

            vec = other.operator->();
            counter = other.getCounterPointer();
            counter->increment();
            return *this;
        }
    }

    Vec2& operator * (){
        return *vec;
    }

    Vec2* operator->(){
        return vec;
    }

    ReferenceCount* getCounterPointer() {
        return counter;
    }

};

unique_ptr_to_vec2 generateUniquePointer() {
    unique_ptr_to_vec2 ptr;
    return ptr;
}


shared_ptr_to_vec2 generateSharedPointer() {
    shared_ptr_to_vec2 ptr;
    return ptr;
}

int main(int argc, char * argv[]){

    // unique_ptr_to_vec2 testPointer;
    unique_ptr_to_vec2 testPointer = generateUniquePointer();
    shared_ptr_to_vec2 sharedTestPointer;
    sharedTestPointer = generateSharedPointer();

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