#include <cstdio>
#include <iostream>
#include <cmath>
#include <ctime>
#include "1d.h"
#include "2d.h"

int main() {
    std::cout << std::endl;
    std::srand(std::time(0));
    short dimension = 2;
    SIZETYPE size = 200;
    if (dimension > 3 || dimension < 1)
        return EXIT_FAILURE;
    if (size <= 0)
        return EXIT_FAILURE;


    TYPE *borders;
    switch (dimension) {
        default:
        case 1:
            borders = new TYPE[2];
            borders[0] = 0;
            borders[1] = 65535;
            calculate1D(size, borders);
        case 2:
            borders = new TYPE[4];
            borders[0] = 1000;
            borders[1] = 5000;
            borders[2] = 4000;
            borders[3] = 8000;
            calculate2D(size, borders);
    }
}