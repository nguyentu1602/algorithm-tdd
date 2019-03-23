# algorithm-tdd
Implement classical algorithms and data structures under test-driven development with proper continuous integration.


Here I develop basic algorithms and data structures in C++ while adhering to a test-driven workflow. I also write-up some attempts of profiling and empirically analyzing complexities of these algorithms & data structures.

Google Gtest and Travis CI are used for automatic compiling and testing. Valgrind is also used for memory leak detection; however, because Valgrind slows tests down by ~20x, I do not include it in Travis CI. Nonetheless, with just one command "make memtest", the memory test could run easily locally.

## Credits
I consulted several books and resources during this project:
  - CLRS's Introduction to Algorithms
  - Miller & Ranum's Problem Solving with Algorithms and Data Structures Using Python
  - M. A. Weiss's Data Structures & Algorithm analysis in C++
  - S. Meyers' Effective C++
  - The C++ STL source code
  - Google Gtest framework documentation and source code

The algorithms and data structures here are all well-known and thus this repo is solely for my learning purpose. Because I used the C++ Standard Template Library regularly at work, I naturally incline to use its containters' interface when implementing my containers. All credits on explaining how the algorithms and data structures work and best practices in implementing them belong to the authors above and all coding errors are mine. Occationaly, I will use examples of complex algorithms implemented by others and only try to write testsuites for them. Under such cases I will adequately cite the source(s) and credit the original author(s).




[![Build Status](https://travis-ci.com/nguyentu1602/algorithm-tdd.svg?branch=master)](https://travis-ci.com/nguyentu1602/algorithm-tdd)
