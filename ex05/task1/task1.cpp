#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include <map>
#include <algorithm>


struct Person {
    std::string first_name;
    std::string last_name;
    int age;

    Person(std::string first, std::string last, int age) : first_name(first),
                                                           last_name(last),
                                                           age(age) {}

    Person( Person& person) : first_name(person.first_name),
                                   last_name(person.last_name),
                                   age(person.age) {}

    Person(const Person& person) : first_name(person.first_name),
                                   last_name(person.last_name),
                                   age(person.age) {}

    Person (Person&& person) {
        first_name = person.first_name;
        last_name = person.last_name;
        age = person.age;
    }

    Person& operator= ( Person&& person) {

        if(this != &person){
            first_name = person.first_name;
            last_name = person.last_name;
            age = person.age;
        }

        return *this;
    }

    Person& operator= ( Person const& person) {

        if(this != &person){
            first_name = person.first_name;
            last_name = person.last_name;
            age = person.age;
        }

        return *this;
    }

    bool operator== (const Person& rhs ) const {
        std::cout << "Operator ==" << std::endl;
        if(first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)
            return true;
        return false;
    }

    bool operator!= (const Person& rhs ) const {
        std::cout << "Operator !=" << std::endl;
        if(first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)
            return false;
        return true;
    }

    bool operator< (const Person& rhs ) const {
        std::cout << "Operator <" << std::endl;
        return std::tie(last_name, first_name, age) < std::tie(rhs.last_name, rhs.first_name, rhs.age);
    }

    bool operator<= (const Person& rhs ) const {
        std::cout << "Operator <=" << std::endl;
        return std::tie(last_name, first_name, age) <= std::tie(rhs.last_name, rhs.first_name, rhs.age);
    }

    bool operator> (const Person& rhs) const {
        std::cout << "Operator >" << std::endl;
        return std::tie(last_name, first_name, age) > std::tie(rhs.last_name, rhs.first_name, rhs.age);
    }

    bool operator>= (const Person& rhs) const {
        std::cout << "Operator >=" << std::endl;
        return std::tie(last_name, first_name, age) >= std::tie(rhs.last_name, rhs.first_name, rhs.age);
    }

    friend std::ostream& operator<< (std::ostream& out, Person& person) {
        out << "Person(" << person.last_name << ", " << person.first_name << ", " << person.age << ")";
        return out;
    }

    std::string to_string() {
        std::cout << "Operator ==" << std::endl;
        return last_name + " " + first_name + " " + std::to_string(age);}
};



int main(){

    Person p1{"A", "Bb", 15};
    Person p2{"B", "Bb", 1};
    Person p3{"B", "Bb", 15};
    Person p4{"A", "Ab", 15};
    Person p5{"A", "Aa", 15};

    std::set<Person> person_set;
    std::vector<Person> person_vector;
    std::map<Person, int> person_map;

    person_set.insert(p1);
    person_set.insert(p2);
    person_set.insert(p3);
    person_set.insert(p4);
    person_set.insert(p5);

    person_vector.push_back(p1);
    person_vector.push_back(p2);
    person_vector.push_back(p3);
    person_vector.push_back(p4);
    person_vector.push_back(p5);

    person_map.insert(std::pair<Person, int>(p1, 1));
    person_map.insert(std::pair<Person, int>(p2, 2));
    person_map.insert(std::pair<Person, int>(p3, 3));
    person_map.insert(std::pair<Person, int>(p4, 4));
    person_map.insert(std::pair<Person, int>(p5, 5));
    std::cout << std::endl;


    std::cout << "========================" << std::endl;

    std::cout << "Set: " << std::endl;
    std::cout << std::endl;
    std::cout << "Find: " << std::endl;
    std::find(person_set.begin(), person_set.end(), p3);
    std::cout << std::endl;
//    std::cout << "Partition: " << std::endl;
    //std::partition(person_set.begin(), person_set.end(), [](Person p){return 0;});
//    std::cout << std::endl;
    std::cout << "For Each: " << std::endl;
    std::for_each(person_set.begin(), person_set.end(), [](Person p){ p.age++; });
    std::cout << std::endl;
    std::cout << "All of: " << std::endl;
    std::for_each(person_set.begin(), person_set.end(), [](Person p){ p.age > 10; });
    std::cout << std::endl;
    std::cout << "Max Element: " << std::endl;
    std::max_element(person_set.begin(), person_set.end());
    std::cout << std::endl;
    std::cout << "MinMax Element: " << std::endl;
    std::minmax_element(person_set.begin(), person_set.end());
    std::cout << std::endl;

    std::cout << "========================" << std::endl;


    std::cout << "Vector: " << std::endl;
    std::cout << std::endl;
    std::cout << "Find: " << std::endl;
    std::find(person_vector.begin(), person_vector.end(), p3);
    std::cout << std::endl;
    std::cout << "Partition: " << std::endl;
    std::partition(person_vector.begin(), person_vector.end(), [](Person p){return p.age%2 == 0;});
    std::cout << std::endl;
    std::cout << "For Each: " << std::endl;
    std::for_each(person_vector.begin(), person_vector.end(), [](Person p){ p.age++; });
    std::cout << std::endl;
    std::cout << "All of: " << std::endl;
    std::for_each(person_vector.begin(), person_vector.end(), [](Person p){ p.age > 10; });
    std::cout << std::endl;
    std::cout << "Max Element: " << std::endl;
    std::max_element(person_vector.begin(), person_vector.end());
    std::cout << std::endl;
    std::cout << "MinMax Element: " << std::endl;
    std::minmax_element(person_vector.begin(), person_vector.end());
    std::cout << std::endl;

    std::cout << "========================" << std::endl;


    std::cout << "Map: " << std::endl;
    std::cout << std::endl;
    std::cout << "Find: " << std::endl;
    std::find(person_map.begin(), person_map.end(), std::pair<const Person, int>(p5, 5));
    std::cout << std::endl;
//    std::cout << "Partition: " << std::endl;
//    std::partition(person_map.begin(), person_map.end(), [](std::pair<Person, int> p){return p.first.age%2 == 0;});
//    std::cout << std::endl;
    std::cout << "For Each: " << std::endl;
    std::for_each(person_map.begin(), person_map.end(), [](std::pair<Person, int> p){ p.first.age++; });
    std::cout << std::endl;
    std::cout << "All of: " << std::endl;
    std::for_each(person_map.begin(), person_map.end(), [](std::pair<Person, int> p){ p.first.age > 10; });
    std::cout << std::endl;
    std::cout << "Max Element: " << std::endl;
    std::max_element(person_map.begin(), person_map.end());
    std::cout << std::endl;
    std::cout << "MinMax Element: " << std::endl;
    std::minmax_element(person_map.begin(), person_map.end());
    std::cout << std::endl;


//    for(auto cb : person_set) {
//        std::cout << cb << std::endl;
//    }
//
//    for(auto cb : person_vector) {
//        std::cout << cb << std::endl;
//    }
//
//    for(auto cb : person_map) {
//        std::cout << cb.first << std::endl;
//    }

}