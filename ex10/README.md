# Exercise Sheet 10 – Libraries

*due on 25 January 2019*

## Task 1 (2 Points)

In case you are doing networking in C++, consider Boost Asio.
For this task, investigate the [chat server example](https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/example/cpp11/chat).

Your presentation should explain the overall architecture and focus on the session management mechanism.
Especially the use of `std::enable_shared_from_this`.

## Task 2 (4 Points)

Implement a simple calculator using either [imgui], [Qt Widgets], or [Qt Quick].

[imgui]: https://github.com/ocornut/imgui
[Qt Widgets]: https://doc.qt.io/qt-5/qtwidgets-index.html
[Qt Quick]: https://doc.qt.io/qt-5/qtquick-index.html

For Qt Quick, the actual calculation needs to be implemented in C++ as we want to investigate the interaction between the GUI framework and the C++ programming language.
