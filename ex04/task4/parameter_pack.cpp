#include <vector>
#include <iostream>

template<typename T>
void append(std::vector<T> &vector, T last){
    vector.push_back(last);
};

template<typename T, typename ...Elements>
void append(std::vector<T> &vector, T t, Elements... elements){
    vector.push_back(t);
    append(vector, elements...);
};

template<typename T, class C>
void append2(std::vector<T> &vector, std::initializer_list<C> list){

    for( auto t : list )
    {
        vector.push_back(t);
    }

};

int main(){
    std::vector<int> xs{1, 2, 3};

    append(xs, 4, 5);
    append(xs, 6, 7, 8);

    std::vector<float> ys{1.0f, 2.0f, 3.0f};

    append(ys, 4.2f, 5.0f);
    append(ys, 6.0f, 7.0f, 8.0f);


    std::cout << "xs: ";
    for(auto &element : xs){
        std::cout << element << ",\t";
    }
    std::cout << "\n";

    std::cout << "ys: ";
    for(auto &element : ys){
        std::cout << element << ",\t";
    }
    std::cout << "\n";

    /*** ALTERNATIVE SOLUTION ***/
    std::vector<double> zs{1, 2, 3};

    append2(zs, {4.2f, 5.0f});
    append2(zs, {6.0f, 7.0f, 8.0f});

    std::cout << "zs: ";
    for(auto &element : zs){
        std::cout << element << ",\t";
    }
    std::cout << "\n";

}
