#include "../src/pi/pi.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

int main(int, char**)
{
    constexpr auto samples = 10'000'000;

    const auto estimated_pi = pi::calculate<pi::method::par>(samples);

    std::cout << estimated_pi << std::endl;

    return round(estimated_pi * 100) == round(M_PI * 100)
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}
