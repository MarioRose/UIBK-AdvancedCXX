#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>


struct Person {
	std::string first_name;
	std::string last_name;
	int age;


	bool operator== (const Person& rhs ) const{
	std::cout << "Operator ==" << std::endl;
	if(first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)
	    return true;
	return false;
	}

	bool operator== (Person& rhs ) const{
	std::cout << "Operator ==" << std::endl;
	if(first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)
	    return true;
	return false;
	}

	bool operator!= (const Person& rhs ) const{
	std::cout << "Operator !=" << std::endl;
	if(first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)
	    return false;
	return true;
	}

	bool operator< (const Person& rhs ) const{
	std::cout << "Operator <" << std::endl;
	return std::tie(last_name, first_name, age) < std::tie(rhs.last_name, rhs.first_name, rhs.age);
	}

	bool operator<= (const Person& rhs ) const{
	std::cout << "Operator <=" << std::endl;
	return std::tie(last_name, first_name, age) <= std::tie(rhs.last_name, rhs.first_name, rhs.age);
	}

	bool operator> (const Person& rhs) const{
	std::cout << "Operator >" << std::endl;
	return std::tie(last_name, first_name, age) > std::tie(rhs.last_name, rhs.first_name, rhs.age);
	}

	bool operator>= (const Person& rhs) const{
	std::cout << "Operator >=" << std::endl;
	return std::tie(last_name, first_name, age) >= std::tie(rhs.last_name, rhs.first_name, rhs.age);
	}

	friend std::ostream& operator<< (std::ostream& out, const Person& person) {
	out << "Person(" << person.last_name << ", " << person.first_name << ", " << person.age << ")";
	return out;
	}

	std::string to_string() const {
	std::cout << "Operator ==" << std::endl;
	return last_name + " " + first_name + " " + std::to_string(age);}
};


std::vector<Person*> getRawPointers(std::vector<std::shared_ptr<Person>>& sharedPointers) {
	std::vector<Person*> rawPointers;

//	//Range based for loop
//	for(auto sharedPointer : sharedPointers) {
//		rawPointers.push_back(sharedPointer.get());
//	}

	//std::transform
//	std::transform(sharedPointers.begin(), sharedPointers.end(),
//		std::back_inserter(rawPointers),
//		[](std::shared_ptr<Person> s){return s.get();});


	//with mem_fn
	auto rawPointer = std::mem_fn(&std::shared_ptr<Person>::get);

	for(auto sharedPointer : sharedPointers) {
		rawPointers.push_back(rawPointer(sharedPointer));
	}


	return rawPointers;
}


int main(){
    	std::shared_ptr<Person> sh1(new Person{"A", "A", 15});
    	std::shared_ptr<Person> sh2(new Person{"B", "B", 18});
    	std::shared_ptr<Person> sh3(new Person{"C", "C", 19});

	std::vector<std::shared_ptr<Person>> sharedPointers = {sh1, sh2, sh3};
	std::vector<Person*> rawPointers;

	//with above function
	rawPointers = getRawPointers(sharedPointers);

//	//with lambda expression
//	auto getRawPointersLambda = [sharedPointers](){
//		std::vector<Person*> returnVec;
//		for(auto sharedPointer : sharedPointers) {
//			returnVec.push_back(sharedPointer.get());
//		}
//		return returnVec;
//	};
//	rawPointers = getRawPointersLambda();


	std::cout << rawPointers[0]->first_name << std::endl;
}