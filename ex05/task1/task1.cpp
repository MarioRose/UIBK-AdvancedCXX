#include <iostream>

struct Person {
    std::string first_name;
    std::string last_name;
    int age;

    bool operator==(const Person& rhs ) const{
        if(first_name.compare(rhs.first_name) && first_name.compare(rhs.first_name) && age == rhs.age)
            return true;
        return false;
    }
};


int main(){
    std::cout << "Hello World!\n";
}