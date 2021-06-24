# Hydraulic Network Library 
(under development)

We are writing a C++ library to perform hydraulic and thermal calculations for nonlinear hydraulic network.

<!---
## Completed:
  
* Parse CSV data into integer multidimensional arrays
      
```
1 -1 0 0 0 0 0 
0 1 -1 0 0 0 0 
0 1 0 -1 0 0 0 
0 0 0 1 0 -1 0 
0 0 0 0 0 1 -1 
0 0 0 0 -1 0 1 
0 0 0 -1 1 0 0 
```

* Derive incidence matrix
    
```
1 -1 0 0 0 0 0 
0 1 -1 0 0 0 0 
0 1 0 -1 0 0 0 
0 0 0 1 0 -1 0 
0 0 0 0 0 1 -1 
0 0 0 0 -1 0 1 
0 0 0 -1 1 0 0 
```

* There are 8760 sets of solutions for heat flows, with each contain 8 values. The first set of solutions is found as
```
57.5494,25.424,32.1254,13.9795,2.43297,-10.9734,13.6895
```
--->

## Status

Completed Hydraulic Simulation, waiting for David for data for full validation.

### To-do

* Full validation of loop detection algorithm and solver

* Tests

Some general ideas for testing science codes (credit to cppslack)

    - Test inputs at boundary conditions

    - Test any inputs that might possibly divide by zero

    - Test inputs for "meaningful" mathematical properties, e.g. if we are doing slop calculations test for a positive and negative slope as well as zero slope

    - Test inputs that challenge numerical precision

    - Additional: fuzz testing may also be appropriate here as it will throw craptons of inputs at our SUT and possibly catch things we didn't think of analytically 
    - A good explanation of fuzz testing can be found on the page for American Fuzzy Lop:
[fuzzy-test](https://afl-1.readthedocs.io/en/latest/)


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


