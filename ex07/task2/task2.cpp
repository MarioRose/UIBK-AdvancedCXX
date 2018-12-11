#include <iostream>
#include <string>
#include <array>
#include <tuple>


template<typename... Types>
struct type_set{
    type_set(Types... types) : set(types...) {}

    std::tuple<Types...> set;
};


template<bool VAL>
struct set_bool {
    static constexpr bool value = VAL;
};

struct true_type : public set_bool<true> {};
struct false_type : public set_bool<false> {};


template<typename T, typename U>
struct is_same : public false_type {};
template<typename T>
struct is_same<T, T> : public true_type {};

template<typename T, typename U>
constexpr int is_same_v = is_same<T, U>::value;



template<class Type_set, class T>
struct type_set_contains {
    // this condition is always false when we reach this case,
    // but we need it to be *dependent*, which e.g. just "false" wouldn't be
            /** TODO: Replace 1 with *dependent* statment (see comment above) **/
    static_assert(1, "type_set_contains called on non-type_set");
};

template<class T>
struct type_set_contains<type_set<>, T> : public false_type {};

template<class T, class Head, class... Rest>
struct type_set_contains<type_set<Head, Rest...>, T> : public set_bool<
        is_same_v<Head, T> || type_set_contains<type_set<Rest...>, T>::value
> {};

template<typename T, typename U>
constexpr int type_set_contains_v = type_set_contains<T, U>::value;


struct B {};

int main(int argc, char const *argv[]) {

    std::cout << "type_set_contains_v<type_set<>, float>:                 " << type_set_contains_v<type_set<>, float> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, char, float>, float>: " << type_set_contains_v<type_set<int, char, float>, float> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, B>, B>:               " << type_set_contains_v<type_set<int, B>, B> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, B>, int>:             " << type_set_contains_v<type_set<int, B>, int> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, B>, float>:           " << type_set_contains_v<type_set<int, B>, float> << std::endl;
    std::cout << "type_set_contains_v<type_set<B>, B>:                    " << type_set_contains_v<type_set<int, B>, B> << std::endl;

//    std::cout << "type_set_contains_v<int, float>: " << type_set_contains_v<int, float> << std::endl;


    return 0;
}
