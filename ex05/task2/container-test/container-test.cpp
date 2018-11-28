#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <ctime>
#include <chrono>

struct Person {
	std::string first_name;
	std::string last_name;
	int age;

	bool operator==(const Person &rhs) const
	{
		if (first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)
			return true;
		return false;
	}
	
	bool operator==(Person &rhs) const
	{
		if (first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)
			return true;
		return false;
	}

	bool operator!=(const Person &rhs) const
	{
		if (first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)
			return false;
		return true;
	}

	bool operator<(const Person &rhs) const
	{
		return std::tie(last_name, first_name, age) < std::tie(rhs.last_name, rhs.first_name, rhs.age);
	}

	bool operator<=(const Person &rhs) const
	{
		return std::tie(last_name, first_name, age) <= std::tie(rhs.last_name, rhs.first_name, rhs.age);
	}

	bool operator>(const Person &rhs) const
	{
		return std::tie(last_name, first_name, age) > std::tie(rhs.last_name, rhs.first_name, rhs.age);
	}

	bool operator>=(const Person &rhs) const
	{
		return std::tie(last_name, first_name, age) >= std::tie(rhs.last_name, rhs.first_name, rhs.age);
	}

	friend std::ostream &operator<<(std::ostream &out, const Person &person)
	{
		out << "Person(" << person.last_name << ", " << person.first_name << ", " << person.age << ")";
		return out;
	}

	std::string to_string() const
	{
		return last_name + " " + first_name + " " + std::to_string(age);
	}
};

namespace std {
  template <>
  struct hash<Person> {
  	size_t operator()(const Person &p) const
  	{
  		return hash<std::string>()(p.first_name + std::to_string(p.age));
  	}
  };
} // namespace std
using namespace std::chrono;

int main(int argc, char *argv[])
{
		
	if(argc < 3) {
		return -1;
	}
		
	const int numElements = atoi(argv[1]);
	const int rMax = atoi(argv[2]);
	const int numTests = atoi(argv[3]);
		
	int insertionTime = 0;
	int lookupTime = 0;
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	
	for( int k = 0; k < numTests; ++k ) { 
	
		std::srand(std::time(nullptr));
		std::unordered_set<Person> person_uset;
		
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		
		for( int i = 0; i < numElements; ++i ) {
			person_uset.insert(Person{std::to_string(std::rand() % rMax), std::to_string(std::rand() % rMax), std::rand() % 100});
		}
		
		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		std::find(person_uset.begin(), person_uset.end(), (Person{std::to_string(std::rand() % rMax), std::to_string(std::rand() % rMax), std::rand() % 100}));
		
		high_resolution_clock::time_point t3 = high_resolution_clock::now();
		
		insertionTime += duration_cast<microseconds>( t2 - t1 ).count();
		lookupTime += duration_cast<microseconds>( t3 - t2 ).count();
	}
	
	
	std::cout << "uset: [" << insertionTime/numTests << "us, " << lookupTime/numTests << "us]" << std::endl;
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
	insertionTime = 0;
	lookupTime = 0;
	
	for( int k = 0; k < numTests; ++k ) { 
	
		std::srand(std::time(nullptr));
		std::set<Person> person_set;
		
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		
		for( int i = 0; i < numElements; ++i ) {
			person_set.insert(Person{std::to_string(std::rand() % rMax), std::to_string(std::rand() % rMax), std::rand() % 100});
		}
		
		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		std::find(person_set.begin(), person_set.end(), (Person{std::to_string(std::rand() % rMax), std::to_string(std::rand() % rMax), std::rand() % 100}));
		
		high_resolution_clock::time_point t3 = high_resolution_clock::now();
		
		insertionTime += duration_cast<microseconds>( t2 - t1 ).count();
		lookupTime += duration_cast<microseconds>( t3 - t2 ).count();
	}
	
	std::cout << "set: [" << insertionTime/numTests << "us, " << lookupTime/numTests << "us]" << std::endl;
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	insertionTime = 0;
	lookupTime = 0;
	
	for( int k = 0; k < numTests; ++k ) { 
	
		std::srand(std::time(nullptr));
		std::map<Person, int> person_map;
		
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		
		for( int i = 0; i < numElements; ++i ) {
			
			int a = std::rand() % rMax;
			int b = std::rand() % rMax;
			
			person_map.insert(
				std::pair<Person, int>(
					Person{std::to_string(a), std::to_string(b), a*10 + b}, (a*10 + b)
				)
			);
		}
		
		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		//auto iterator = number_map.find(std::rand() % rMax);
		//std::find(person_map.begin(), person_map.end(), std::pair<Person, int>(, (std::rand() % numElements)));
		
		int a = std::rand() % rMax;
		int b = std::rand() % rMax;
		auto pTmp = Person{std::to_string(a), std::to_string(b), a*10 + b};
		
		std::find(person_map.begin(), person_map.end(), std::pair<const Person, int>(pTmp, a*10 + b));
		
		high_resolution_clock::time_point t3 = high_resolution_clock::now();
		
		insertionTime += duration_cast<microseconds>( t2 - t1 ).count();
		lookupTime += duration_cast<microseconds>( t3 - t2 ).count();
	}
	
	std::cout << "map: [" << insertionTime/numTests << "us, " << lookupTime/numTests << "us]" << std::endl;
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
	insertionTime = 0;
	lookupTime = 0;
	
	for( int k = 0; k < numTests; ++k ) { 
	
		std::srand(std::time(nullptr));
		std::vector<Person> person_vector;
		
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		
		for( int i = 0; i < numElements; ++i ) {
			person_vector.push_back(Person{std::to_string(std::rand() % rMax), std::to_string(std::rand() % rMax), std::rand() % 100});
		}
		
		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		std::find(person_vector.begin(), person_vector.end(), (Person{std::to_string(std::rand() % rMax), std::to_string(std::rand() % rMax), std::rand() % 100}));
		
		high_resolution_clock::time_point t3 = high_resolution_clock::now();
		
		insertionTime += duration_cast<microseconds>( t2 - t1 ).count();
		lookupTime += duration_cast<microseconds>( t3 - t2 ).count();
	}
	
	std::cout << "vector: [" << insertionTime/numTests << "us, " << lookupTime/numTests << "us]" << std::endl;

	
	return 0;
}
