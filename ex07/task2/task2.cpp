#include <iostream>
#include <string>
#include <array>
#include <tuple>


template<typename... Types>
struct type_set{
};


template<bool VAL>
struct set_bool {
    static constexpr bool value = VAL;
};

struct true_type : set_bool<true> {};
struct false_type : set_bool<false> {};


template<typename T, typename U>
struct is_same : false_type {};
template<typename T>
struct is_same<T, T> : true_type {};

template<typename T, typename U>
constexpr int is_same_v = is_same<T, U>::value;



// CONTAINS

template<class Type_set, class T>
struct type_set_contains {
    static_assert(sizeof(Type_set) == -1, "type_set_contains called on non-type_set");
};

template<class T>
struct type_set_contains<type_set<>, T> : false_type {};

template<class T, class Head, class... Rest>
struct type_set_contains<type_set<Head, Rest...>, T> : set_bool<
        is_same_v<Head, T> || type_set_contains<type_set<Rest...>, T>::value
> {};

template<typename T, typename U>
constexpr int type_set_contains_v = type_set_contains<T, U>::value;

// --- IS SUBSET ---------------------------------------------------------
template<class Type_set, class T>
struct type_set_is_subset {
    static_assert(sizeof(Type_set) == -1, "type_set_is_subset called on non-type_set");
};

//template<class T, class U>
//struct type_set_is_subset : false_type {};

template<>
struct type_set_is_subset<type_set<>, type_set<>> : true_type {};

template<class Head, class... T>
struct type_set_is_subset<type_set<Head, T...>, type_set<>> : false_type {};

template<class Head, class... T>
struct type_set_is_subset<type_set<>, type_set<Head, T...>> : true_type {};

template<class... T>
struct type_set_is_subset<type_set<>, type_set<T...>> : true_type {};

template<class Head, class... T>
struct type_set_is_subset<type_set<Head, T...>, type_set<Head, T...>> : true_type {};

template<class... T, class Head, class... Rest>
struct type_set_is_subset<type_set<Head, Rest...>, type_set<T...>> : set_bool<
        type_set_contains_v<type_set<T...>, Head> && type_set_is_subset<type_set<Rest...>, type_set<T...>>::value
> {};

template<typename T, typename U>
constexpr int type_set_is_subset_v = type_set_is_subset<T, U>::value;
// --- END IS SUBSET ---------------------------------------------------------

// IS SAME
template<class Type_set, class T>
struct type_set_is_same {
    static_assert(sizeof(Type_set) == -1, "type_set_is_same called on non-type_set");
};

//template<class T, class U>
//struct type_set_is_same : false_type {};

template<class T>
struct type_set_is_same<T, type_set<>> : false_type {};

template<class T>
struct type_set_is_same<type_set<>, T> : false_type {};


template<class T>
struct type_set_is_same<type_set<>, type_set<T>> : false_type {};

template<class T>
struct type_set_is_same<type_set<T>, type_set<>> : false_type {};

template<class... T>
struct type_set_is_same<type_set<>, type_set<T...>> : false_type {};

template<class... T>
struct type_set_is_same<type_set<T...>, type_set<>> : false_type {};

template<>
struct type_set_is_same<type_set<>, type_set<>> : true_type {};


template<class Head_1, class... Rest_1, class Head_2, class... Rest_2>
struct type_set_is_same<type_set<Head_1, Rest_1...>, type_set<Head_2, Rest_2...>> : set_bool<
        is_same_v<Head_1, Head_2> && type_set_is_same<type_set<Rest_1...>, type_set<Rest_2...>>::value
> {};

template<typename T, typename U>
constexpr int type_set_is_same_v = type_set_is_same<T, U>::value;

// TYPESET SIZE
template<typename T>
struct type_set_size {
	static constexpr int value = 0;
};

template<class Head, class... T>
struct type_set_size<type_set<Head, T...>> {
    //
	static constexpr int value =
        (type_set_is_subset_v<type_set<Head>, type_set<T...>> ? 0 : 1) +
        type_set_size<type_set<T...>>::value;
};

template<typename T>
constexpr int type_set_size_v = type_set_size<T>::value;

struct B {};

int main(int argc, char const *argv[]) {

    std::cout << "___________CONTAINS___________" << std::endl;

    std::cout << "type_set_contains_v<type_set<>, float>:                 " << type_set_contains_v<type_set<>, float> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, char, float>, float>: " << type_set_contains_v<type_set<int, char, float>, float> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, B>, B>:               " << type_set_contains_v<type_set<int, B>, B> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, B>, int>:             " << type_set_contains_v<type_set<int, B>, int> << std::endl;
    std::cout << "type_set_contains_v<type_set<int, B>, float>:           " << type_set_contains_v<type_set<int, B>, float> << std::endl;
    std::cout << "type_set_contains_v<type_set<B>, B>:                    " << type_set_contains_v<type_set<int, B>, B> << std::endl;

    // Example of call with non-type_set - doesn't Compile
    //std::cout << "type_set_contains_v<int, float>: " << type_set_contains_v<int, float> << std::endl;
    std::cout << std::endl << std::endl;

    std::cout << "___________IS_SUBSET___________" << std::endl;

    std::cout << "type_set_is_subset_v<type_set<int, float>, type_set<int, float>>:       " << type_set_is_subset_v<type_set<int, float>, type_set<int, float>> << std::endl;
    std::cout << "type_set_is_subset_v<type_set<>, type_set<>>:                           " << type_set_is_subset_v<type_set<>, type_set<>> << std::endl;
    std::cout << "type_set_is_subset_v<type_set<>, type_set<int>>:                        " << type_set_is_subset_v<type_set<>, type_set<int>> << std::endl;
    std::cout << "type_set_is_subset_v<type_set<int>, type_set<>>:                        " << type_set_is_subset_v<type_set<int>, type_set<>> << std::endl;
    std::cout << "type_set_is_subset_v<type_set<int, float>, type_set<>>:                 " << type_set_is_subset_v<type_set<int, float>, type_set<>> << std::endl;
    std::cout << "type_set_is_subset_v<type_set<int, float, B>, type_set<int, float>>:    " << type_set_is_subset_v<type_set<int, float, B>, type_set<int, float>> << std::endl;
    std::cout << "type_set_is_subset_v<type_set<int, float, B>, type_set<int, float, B>>: " << type_set_is_subset_v<type_set<int, float, B>, type_set<int, float, B>> << std::endl;
    std::cout << "type_set_is_subset_v<type_set<int, float, B>, type_set<B, int, float>>: " << type_set_is_subset_v<type_set<int, float, B>, type_set<B, int, float>> << std::endl;
    std::cout << "type_set_is_subset_v<type_set<int, float>, type_set<int, float, B>>: " << type_set_is_subset_v<type_set<int, float>, type_set<int, float, B>> << std::endl;

    // Example of call with non-type_set - doesn't Compile
    //std::cout << "type_set_is_subset_v<int, float>: " << type_set_is_subset_v<int, float> << std::endl;
    std::cout << std::endl << std::endl;


    std::cout << "___________IS_SAME___________" << std::endl;
    std::cout << "type_set_is_same_v<type_set<int, float>, type_set<int, float>>:       " << type_set_is_same_v<type_set<int, float>, type_set<int, float>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<>, type_set<>>:                           " << type_set_is_same_v<type_set<>, type_set<>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<>, type_set<int>>:                        " << type_set_is_same_v<type_set<>, type_set<int>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<int, float>, type_set<>>:                 " << type_set_is_same_v<type_set<int, float>, type_set<>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<int, float, B>, type_set<int, float>>:    " << type_set_is_same_v<type_set<int, float, B>, type_set<int, float>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<int, float, B>, type_set<int, float, B>>: " << type_set_is_same_v<type_set<int, float, B>, type_set<int, float, B>> << std::endl;
    std::cout << "type_set_is_same_v<type_set<int, float, B>, type_set<B, int, float>>: " << type_set_is_same_v<type_set<int, float, B>, type_set<B, int, float>> << std::endl;

    std::cout << std::endl << std::endl;

    std::cout << "___________SIZE___________" << std::endl;
    std::cout << "type_set_size_v<type_set<>>:                          "   << type_set_size_v<type_set<>> << std::endl;
    std::cout << "type_set_size_v<type_set<int>>:                       "   << type_set_size_v<type_set<int>> << std::endl;
    std::cout << "type_set_size_v<type_set<int, int>>:                  "   << type_set_size_v<type_set<int, int>> << std::endl;
    std::cout << "type_set_size_v<type_set<int, float>>:                "   << type_set_size_v<type_set<int, float>> << std::endl;
    std::cout << "type_set_size_v<type_set<int, int, float>>:           "   << type_set_size_v<type_set<int, int, float>> << std::endl;
    std::cout << "type_set_size_v<type_set<int, int, float, float>>:    "   << type_set_size_v<type_set<int, int, float, float>> << std::endl;
    std::cout << "type_set_size_v<type_set<int, int, float, int>>:      "   << type_set_size_v<type_set<int, int, float, int>> << std::endl;


    std::cout << std::endl << std::endl;


    return 0;
}
