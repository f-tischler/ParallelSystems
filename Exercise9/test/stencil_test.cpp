//
// Created by ivan on 11/12/17.
//

#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>

#define N768

#include "../mpi_stencil.h"
#include "sequential/grid_helper.h"
#include "sequential/jacobi.h"
#include "sequential/iteration.h"
#include "sequential/stencil_code.h"
#include "sequential/print.h"

using stencil_code_t = stencil_code::stencil_iteration<stencil_code::jacobi>;

int main(int argc, char **argv)
{
    auto my_rank = 0;
    auto num_procs = 1;

    const auto blocks = static_cast<int>(sqrt(num_procs));

    const std::array<const double, 4> borders
    {
            1.0, 0.5, 0, -0.5
    };

    grid grid1(my_rank, blocks, borders);
    grid grid2(my_rank, blocks, borders);

    const ::stencil s(num_procs, my_rank, grid1);

    const auto iterations = s.execute(grid1, grid2);

    constexpr auto epsilon = 10;
    constexpr auto n = 768;

    constexpr ::bounds_t<2> bounds = { 1.0, 0.5, 0, -0.5 };
    const stencil_code::grid_extents_t<2> extents = { n, n};

    auto grid_old = create_buffered_grid(extents, bounds, default_value);
    auto iterations_old = 0;

    for(;;)
    {
        const auto result = stencil_code_t::do_iteration(grid_old, epsilon);

        iterations_old += std::get<0>(result);

        if (std::get<1>(result)) break;
    }

    decltype(auto) first = stencil_code::get_first(grid_old);
  
    for (auto i = 0; i < n; ++i) {
        for (auto j = 0; j < n; ++j) {
            if(std::abs(first.at({i,j}) - grid2.get(i,j)) > 0.5)
                std::cout << "Error at " << i << "|" << j 
                          << ", old: " << first.at({i,j}) 
                          << "    new: " << grid2.get(i,j)
                          << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
