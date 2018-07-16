# CPPTutorial
A CPP tutorial that introduce the C++11, C++14 feature, googletest, CMake and the Design patterns.

This tutorial is based on a simple system that simulates the data tranfer from different Network Elements in wireless network.

The basic idea of this tutorial is:
1) Use Factory pattern to create different Network Elements.
2) Use Factory pattern to create different protocol stacks that the Network element supports.
3) Use command pattern to use each protocol class instance in protocol stacks and different NE.
4) Use traits or policy pattern to specify that which protocol stacks that NE shall use.
5) Provides logs system to class instance to check the class creation and destory
6) Using googleTest as TDD practice.
7) Using CMake to build the system with different target.
8) Try to use this tutorial as an example or playground to pratice C++11, C++14 feature and metaprogramming.
