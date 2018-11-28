#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <tuple>
#include <unordered_set>
#include <vector>

/*
std::unordered_set internally implements a hash table to store the elements

There are 2 ways to make an unordered_set of User Define Types / Classes i.e.

 - Create special functions to make std::hash<> & std::equals_to<>
    to work with User Defined classes
 - Creating Custom Hasher and Comparision Functors and pass it to unordered_set.


*/

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

int main()
{

	std::unordered_set<Person> person_uset;

	for( int i = 0; i < 4000; ++i ) {
		person_uset.insert(Person{"A", "Aa", 15});
	}

	std::find(person_uset.begin(), person_uset.end(), Person{"A", "Aa", 15});
}
