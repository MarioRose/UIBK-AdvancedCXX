#include <iostream>
#include <set>

struct Person {
    std::string first_name;
    std::string last_name;
    int age;


    bool operator== (const Person& rhs ) const{
        if(first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)
            return true;
        return false;
    }

    bool operator!= (const Person& rhs ) const{
        if(first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)
            return false;
        return true;
    }

    bool operator< (const Person& rhs ) const{
        if(this == &rhs)
            return false;

        if(last_name < rhs.last_name)
            return true;
        if(last_name == rhs.last_name && first_name < rhs.first_name)
            return true;
        if(last_name == rhs.last_name && first_name == rhs.first_name && age < rhs.age)
            return true;

        return false;
    }

    bool operator<= (const Person& rhs ) const{
        if(this == &rhs)
            return true;

        if(last_name <= rhs.last_name)
            return true;
        if(last_name == rhs.last_name && first_name <= rhs.first_name)
            return true;
        if(last_name == rhs.last_name && first_name == rhs.first_name && age <= rhs.age)
            return true;

        return false;
    }

    bool operator> (const Person& rhs) const{
        if(this == &rhs)
            return false;
        if(this < &rhs)
            return false;
        return true;
    }

    bool operator>= (const Person& rhs) const{
        if(this == &rhs)
            return true;
        if(this < &rhs)
            return false;
        return true;
    }

    friend std::ostream& operator<< (std::ostream& out, const Person& person) {
        out << "Person(" << person.last_name << ", " << person.first_name << ", " << person.age << ")";
        return out;
    }

    std::string to_string() const {return last_name + " " + first_name + " " + std::to_string(age);}
};


int main(){

    Person p1{"A", "Bb", 15};
    Person p2{"B", "Bb", 1};
    Person p3{"B", "Bb", 15};
    Person p4{"A", "Aa", 15};
    Person p5{"A", "Aa", 15};

    std::set<Person> person_set;

    person_set.insert(p1);
    person_set.insert(p2);
    person_set.insert(p3);
    person_set.insert(p4);
    person_set.insert(p5);


    for(auto cb : person_set) {
        std::cout << cb << std::endl;
    }
    
}