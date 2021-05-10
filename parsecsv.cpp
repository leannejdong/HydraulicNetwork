#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main (int argc, char **argv) {

    std::ifstream f;

    if (argc > 1) {         /* if argument given */
        f.open (argv[1]);   /* open file with filename as argument */
        if (! f.is_open()) {    /* validate file open for reading */
            std::cerr << "error: file open failed '" << argv[1] << "'.\n";
            return 1;
        }
    }
    else {  /* no argument given, error show usage */
        std::cerr << "error: insufficient input. <filename> required.\n";
        return 1;
    }

    std::string line, val;                  /* string for line & value */
    std::vector<std::vector<int>> array;    /* vector of vector<int>  */

    while (std::getline (f, line)) {        /* read each line */
        std::vector<int> v;                 /* row vector v */
        std::stringstream s (line);         /* stringstream line */
        while (getline (s, val, ','))       /* get each value (',' delimited) */
            v.push_back (std::stoi (val));  /* add to row vector */
        array.push_back (v);                /* add row vector to array */
    }
    for (auto& row : array) {               /* iterate over rows */
      for (auto& val : row)               /* iterate over vals */
        std::cout << val << "  ";       /* output value      */
      std::cout << "\n";                  /* tidy up with '\n' */
    }
}
