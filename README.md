# Hydraulic Network Library 
(under development)

We start writing a C++ library for solving nonlinear hydraulic network.

## Completed:
  
* Parse CSV data into integer multidimensional arrays

* Derive incidence matrix

* Cycles detected
```shell
12 58 57 18 17 16 15 14 13 12 
32 40 39 38 37 36 35 34 33 32 
42 67 66 65 64 63 62 61 60 59 58 57 18 30 32 33 34 35 36 37 38 39 41 42 
53 69 68 67 66 65 64 63 62 61 60 59 58 57 18 30 32 33 34 35 36 37 38 39 41 42 50 52 53 
54 57 18 30 32 33 34 35 36 37 38 39 41 42 50 52 53 54 
62 69 68 67 66 65 64 63 62 
63 83 82 80 77 75 74 72 71 70 64 63 
82 98 97 95 94 93 91 90 89 86 85 84 80 82 
85 98 97 95 94 93 91 90 89 86 85 
94 99 59 60 61 62 63 64 70 71 72 74 75 77 80 84 85 86 89 90 91 93 94 
Timer took in s 44.0473s
Timer took in s 44.0591s
Process finished with exit code 0
```
<!---
* There are 8760 sets of solutions for heat flows, with each contain 8 values. The first set of solutions is found as
```
57.5494,25.424,32.1254,13.9795,2.43297,-10.9734,13.6895
```
-->

## Build Instruction

### Dependency required

- CMake = 3.12
- A Modern C++ compiler
- Eigen3.3
- doctest

  [The fastest feature-rich C++11/14/17/20 single-header testing framework](https://raw.githubusercontent.com/onqtam/doctest/master/doctest/doctest.h)

### Linux

#### Build from terminal

```
git clone git@github.com:leannejdong/HydraulicNetwork.git
cd HydraulicNetwork/
mkdir build
cd build
cmake ..
make
```
To run the main program
```
cd ..
build/HydraulicNetwork
```
To run the test suit
```
ctest
```

#### Build from CLion (Best choice for windows user)

Open the project directory and hit `shift-f10`


