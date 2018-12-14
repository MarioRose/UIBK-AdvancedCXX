#include <iostream>
#include <string>
#include <array>

template <std::size_t N>
struct Attributes
{
  // default empty
};

template <>
struct Attributes<1>
{
	int x = 0;
};

template <>
struct Attributes<2>
{
	int x = 0;
	int y = 0;
};

template <>
struct Attributes<3>
{
	int x = 0;
	int y = 0;
	int z = 0;
};




template <std::size_t N>
class Vector : public Attributes<N> {
  public:

    Vector(){}

    template <typename... Args>
    Vector(Args... args) : data{ double(args)... }
    {
        static_assert(sizeof...(Args) == N, "wrong number of arguments");

        if constexpr(N == 1)
		{
			this->x = this->data[0];
		}
		else if constexpr(N == 2)
		{
			this->x = this->data[0];
			this->y = this->data[1];
		}
		else if constexpr(N == 3)
		{
			this->x = this->data[0];
			this->y = this->data[1];
			this->z = this->data[2];
		}

    }

    // copy constructor
    Vector(Vector &v)
    {
        std::cout << "Copy constructor called" << '\n';
        for (int i=0; i < N; i++)
        {
          data[i] = v.data[i];
        }
    }

    // copy assignment operator
    Vector& operator=(const Vector &v)
    {
        std::cout << "Copy assignment called" << '\n';

        for (int i=0; i < N; i++)
        {
          data[i] = v.data[i];
        }
        return *this;
    }

    // Move constructor
    Vector(Vector&& other)
       : data({0})
    {
        std::cout << "Move constructor called" << '\n';
        // Copy the data pointer and its length from the
        // source object.
        data = other.data;

        // Release the data pointer from the source object so that
        // the destructor does not free the memory multiple times.
        other.data = {0};
    }


    // move assignment
    Vector& operator=(Vector&& other)
    {

        std::cout << "Move assignment called" << '\n';
        if (this != &other)
        {
            // Copy the data pointer and its length from the
            // source object.
            data = other.data;

            // Release the data pointer from the source object so that
            // the destructor does not free the memory multiple times.
            other.data = {0};
        }

        return *this;
    }


    // Overload + operator to add two Box objects.
    Vector operator+(const Vector& v) {
        Vector vector;
        for (int i=0; i < N; i++)
        {
          vector.data[i] = this->data[i] + v.data[i];
        }
        return vector;
    }

    double operator[](const int i) const
    {
        return this->data[i];
    }

    std::string to_string()
    {
        std::string s = "[";
        for(const auto& d : data)
        {
            s += std::to_string(d) + ", ";
        }
        // remove last  ,_ and replace with ]
        return s.substr(0, s.find_last_of(',')) + "]";
    }

  private:
    std::array<double, N> data = {0};
};

int main(int argc, char const *argv[]) {

    using Vec1 = Vector<1>;
    using Vec2 = Vector<2>;
    using Vec3 = Vector<3>;

    std::cout << "___________Vector<1>___________" << std::endl;
    Vec1 v1(10);
    Vec1 v1_2;
    std::cout << (v1 + v1_2).to_string() << std::endl << std::endl;
    std::cout << std::to_string(v1.x) << std::endl;

    std::cout << "___________Vector<2>___________" << std::endl;
    Vec2 v2(1,5);
    Vec2 v2_2(v2);
    Vec2 v2_3;
    v2_3 = v2;
    std::cout << v2_2.to_string() << std::endl;
    std::cout << v2_3.to_string() << std::endl;
    std::cout << std::to_string(v2[1]) << " =?= " << std::to_string(v2.y) << std::endl << std::endl;

    std::cout << "___________Vector<3>___________" << std::endl;
    Vec3 v3(2,6,1);
    Vec3 v3_2(1,5,4);
    std::cout << (v3 + v3_2).to_string() << std::endl;
    std::cout << std::to_string(v3[0]) << std::endl;
    std::cout << std::to_string(v3[2]) << " =?= " << std::to_string(v3.z) << std::endl << std::endl;

    return 0;
}
