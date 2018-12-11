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



// Contains:

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


// Is Same:

template<class Type_set, class T>
struct type_set_is_same {
    // this condition is always false when we reach this case,
    // but we need it to be *dependent*, which e.g. just "false" wouldn't be
    /** TODO: Replace 1 with *dependent* statment (see comment above) **/
    static_assert(1, "type_set_is_same called on non-type_set");
};


template<class T>
struct type_set_is_same<T, type_set<>> : public false_type {};

template<class T>
struct type_set_is_same<type_set<>, T> : public false_type {};


template<class T>
struct type_set_is_same<type_set<>, type_set<T>> : public false_type {};

template<class T>
struct type_set_is_same<type_set<T>, type_set<>> : public false_type {};

template<class... T>
struct type_set_is_same<type_set<>, type_set<T...>> : public false_type {};

template<class... T>
struct type_set_is_same<type_set<T...>, type_set<>> : public false_type {};

template<>
struct type_set_is_same<type_set<>, type_set<>> : public true_type {};


template<class Head_1, class... Rest_1, class Head_2, class... Rest_2>
struct type_set_is_same<type_set<Head_1, Rest_1...>, type_set<Head_2, Rest_2...>> : public set_bool<
        is_same_v<Head_1, Head_2> && type_set_is_same<type_set<Rest_1...>, type_set<Rest_2...>>::value
> {};
template<typename T, typename U>
constexpr int type_set_is_same_v = type_set_is_same<T, U>::value;




struct B {};

int main(int argc, char const *argv[]) {

    std::cout << "___________Contains:___________" << std::endl;

    std::cout << "type_set_contains_v<type_set<>, float>:                 " << type_set_contains_v<type_set<>, float> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, char, float>, float>: " << type_set_contains_v<type_set<int, char, float>, float> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, B>, B>:               " << type_set_contains_v<type_set<int, B>, B> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, B>, int>:             " << type_set_contains_v<type_set<int, B>, int> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, B>, float>:           " << type_set_contains_v<type_set<int, B>, float> << std::endl;
    std::cout << "type_set_contains_v<type_set<B>, B>:                    " << type_set_contains_v<type_set<int, B>, B> << std::endl;

    //    std::cout << "type_set_contains_v<int, float>: " << type_set_contains_v<int, float> << std::endl;
    std::cout << std::endl << std::endl;


    std::cout << "___________Is_Same:___________" << std::endl;
    std::cout << "type_set_is_same_v<type_set<int, float>, type_set<int, float>>:       " << type_set_is_same_v<type_set<int, float>, type_set<int, float>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<>, type_set<>>:                           " << type_set_is_same_v<type_set<>, type_set<>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<>, type_set<int>>:                        " << type_set_is_same_v<type_set<>, type_set<int>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<int, float>, type_set<>>:                 " << type_set_is_same_v<type_set<int, float>, type_set<>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<int, float, B>, type_set<int, float>>:    " << type_set_is_same_v<type_set<int, float, B>, type_set<int, float>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<int, float, B>, type_set<int, float, B>>: " << type_set_is_same_v<type_set<int, float, B>, type_set<int, float, B>> << std::endl;

    std::cout << std::endl << std::endl;


    return 0;
}
