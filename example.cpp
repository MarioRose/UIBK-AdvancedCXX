#include <iostream>

void foo(void*){
	std::cout << "void" << std::endl;
}


template<class t>
void temp(){
	foo(0);
}

void foo(float){
	std::cout << "double" << std::endl;
}

void foo(int){
	std::cout << "int" << std::endl;
}


int main(int argc, char* argv[]){
	temp<int>();
	temp<double>();
}
