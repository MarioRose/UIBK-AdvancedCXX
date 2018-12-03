# Exercise Sheet 6

*due on 7 December 2018*

Since we did not yet start with template meta programming, let's have two tasks dedicated to dynamic loading.

## Task 1 (3 Points)

The folder `task1` contains code and build instructions for a shared library and an executable which uses the shared library.
The shared library features two functions `random_number` and `just_a_print` inside the `foo` namespace.

Your task is to create an *interceptor* library:

- `random_number` should be replaced with a function that always returns `4` for improved determinism
- `just_a_print` should be wrapped so that some text is printed before and after the original function is executed

Running the executable with and without the interceptor library could look like this:

    $ ./executable
    Random Number: 880806932

    Just a print to stdout, nothing else

    $ LD_PRELOAD=$PWD/interceptor.so ./executable
    Random Number: 4

    some text before
    Just a print to stdout, nothing else
    some text after

*Hint:* For Linux, have a look at the related man-pages like *ld-linux(8)*.

## Task 2 (2 Points)

In this task you have to create a rudimentary plugin system.

You are given `plugin.hpp` which contains an interface for your plugins, as well as the function name of the constructor function and its type.
Note that the constructor function returns an `std::unique_ptr<Plugin>`.

- create an executable which *dynamically* loads plugins and executes their `run` member function
- create two different plugins (`foo` and `bar`) showing off the plugin system

It could look like this:

    $ ./main ./libFoo.so
    Creation of first plugin
    Running the first plugin
    Destruction of first plugin

    $ ./main ./libFoo.so ./libBar.so
    Creation of first plugin
    Running the first plugin
    Destruction of first plugin
    Creation of second plugin
    Running the second plugin
    Destruction of second plugin

*Hint:* Again have a look at the related man-pages *dlopen(3)* and *dlsym(3)*.
