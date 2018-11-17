#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


struct Person {
	std::string first_name;
	std::string last_name;
	int age;
};

bool operator ==(const Person& lhs, const Person& rhs) {
	return lhs.first_name == rhs.first_name
	       && lhs.last_name == rhs.last_name && lhs.age == rhs.age;
}

class Room {
public:
	Room(int id, size_t limit) : id{id}, limit{limit} {}
	// Returns true iff the person successfully entered.
	bool enter(Person person) {
		if(people.size() < limit) {
			people.push_back(person);
			std::cout << "Person " << person.first_name << " "
			<< person.last_name << " entered Room with ID: " << id << std::endl;
			return true;
		}
		std::cout << "Person " << person.first_name << " "
		<< person.last_name << " was not able to enter Room with ID: " << id
		<< " because it's person limit is reached" << std::endl;
		return false;
	}
	void exit(Person person) {
		std::cout << "Person " << person.first_name << " "
		<< person.last_name << " left Room with ID: " << id << std::endl;
		people.erase(std::remove(people.begin(), people.end(), person), people.end());
	}

	size_t getNumberOfPeople() const {
		return people.size();
	}

	bool isInRoom(Person person) const {
		if(std::find(people.begin(), people.end(), person) != people.end()){
			std::cout << "Person " << person.first_name << " "
			<< person.last_name << " is located in Room with ID: " << id << std::endl;
			return true;
		}
		std::cout << "Person " << person.first_name << " "
		<< person.last_name << " is not located in Room with ID: " << id << std::endl;
		return false;
	}

	std::vector<Person> getPeople() const {
		return people;
	}

private:
	const int id;
	const size_t limit;
	std::vector<Person> people;
};


int main(int argc, char * argv[]){

	Person p1;
	p1.first_name = "Max";
	p1.last_name = "Mustermann";
	p1.age = 21;

	Room r1{1, 5};
	r1.enter(p1);
	r1.isInRoom(p1);
	r1.exit(p1);
	r1.isInRoom(p1);

	Room r2{2, 1};
	Person p2;
	p2.first_name = "Muster";
	p2.last_name = "Maxmann";
	p2.age = 22;

	r2.enter(p1);
	r2.enter(p2);
}