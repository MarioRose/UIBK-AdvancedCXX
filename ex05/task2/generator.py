#!/usr/bin/python
import sys
import random
import names

if(len(sys.argv) < 3):
    print("please provide number of persons and datastructure type!")
    exit

if(sys.argv[1] != "map" and sys.argv[1] != "vector" and
    sys.argv[1] != "set" and sys.argv[1] != "uset"):
    print("not a valid datastructure!")
    exit

f = open("test/"+str(sys.argv[1])+"-"+str(sys.argv[2])+".cpp", "w")

f.write("\
#include <algorithm>\n\
#include <iostream>\n\
#include <map>\n\
#include <set>\n\
#include <tuple>\n\
#include <unordered_set>\n\
#include <vector>\n\
\n\
struct Person {\n\
	std::string first_name;\n\
	std::string last_name;\n\
	int age;\n\
\n\
	bool operator==(const Person &rhs) const\n\
	{\n\
		if (first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)\n\
			return true;\n\
		return false;\n\
	}\n\
\n\
	bool operator!=(const Person &rhs) const\n\
	{\n\
		if (first_name == rhs.first_name && last_name == rhs.last_name && age == rhs.age)\n\
			return false;\n\
		return true;\n\
	}\n\
\n\
	bool operator<(const Person &rhs) const\n\
	{\n\
		return std::tie(last_name, first_name, age) < std::tie(rhs.last_name, rhs.first_name, rhs.age);\n\
	}\n\
\n\
	bool operator<=(const Person &rhs) const\n\
	{\n\
		return std::tie(last_name, first_name, age) <= std::tie(rhs.last_name, rhs.first_name, rhs.age);\n\
	}\n\
\n\
	bool operator>(const Person &rhs) const\n\
	{\n\
		return std::tie(last_name, first_name, age) > std::tie(rhs.last_name, rhs.first_name, rhs.age);\n\
	}\n\
\n\
	bool operator>=(const Person &rhs) const\n\
	{\n\
		return std::tie(last_name, first_name, age) >= std::tie(rhs.last_name, rhs.first_name, rhs.age);\n\
	}\n\
\n\
	friend std::ostream &operator<<(std::ostream &out, const Person &person)\n\
	{\n\
		out << \"Person(\" << person.last_name << \", \" << person.first_name << \", \" << person.age << \")\";\n\
		return out;\n\
	}\n\
\n\
	std::string to_string() const\n\
	{\n\
		return last_name + \" \" + first_name + \" \" + std::to_string(age);\n\
	}\n\
};\n\
\n\
namespace std {\n\
  template <>\n\
  struct hash<Person> {\n\
  	size_t operator()(const Person &p) const\n\
  	{\n\
  		return hash<std::string>()(p.first_name + std::to_string(p.age));\n\
  	}\n\
  };\n\
} // namespace std\n\
\n\
int main()\n{\n\
")


for i in range(0, int(sys.argv[2])):
    ra = random.randint(1,100)
    f.write("   Person p"+str(i)+"{\""+names.get_first_name()+"\",\""+names.get_last_name()+"\", "+str(ra)+"};\n")

if(sys.argv[1] == "uset"):
    f.write("   std::unordered_set<Person> person_uset;\n");

    for i in range(0, int(sys.argv[2])):
        f.write("   person_uset.insert(p"+str(i)+");\n")

    f.write("\n //std::find(person_uset.begin(), person_uset.end(), p"+str(random.randint(0,int(sys.argv[2])-1))+");\n}")

elif(sys.argv[1] == "map"):
    f.write("   std::map<Person, int> person_map;\n");
    for i in range(0, int(sys.argv[2])):
        f.write("   person_map.insert(std::pair<Person, int>(p"+str(i)+", "+str(i)+"));\n")

    r = random.randint(0,int(sys.argv[2])-1)
    f.write("\n //std::find(person_map.begin(), person_map.end(), std::pair<const Person, int>(p"+str(r)+", "+str(r)+"));\n}")

elif(sys.argv[1] == "vector"):
    f.write("   std::vector<Person> person_vector;\n");
    for i in range(0, int(sys.argv[2])):
        f.write("   person_vector.push_back(p"+str(i)+");\n")

    r = random.randint(0,int(sys.argv[2])-1)
    f.write("\n //std::find(person_vector.begin(), person_vector.end(), p"+str(r)+");\n}")

elif(sys.argv[1] == "set"):
    f.write("   std::map<Person, int> person_map;\n");
    for i in range(0, int(sys.argv[2])):
        f.write("   person_set.insert(p"+str(i)+");\n")

    r = random.randint(0,int(sys.argv[2])-1)
    f.write("\n //std::find(person_set.begin(), person_set.end(), p"+str(r)+");\n}")
