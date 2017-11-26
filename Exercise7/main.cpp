#include "grid.h"
#include "print.h"
#include "grid_helper.h"
#include "chrono_timer.h"
#include "jacobi.h"
#include "iteration.h"

#include <string>
#include <iostream>

// ==============================================================================
// define types
//using cell_t = float;
using cell_t = std::array<float, 2>;
using cell_value_t = cell_t::value_type;

template<size_t Dim>
using grid_t = stencil::grid_t<cell_t, Dim>;

template<size_t Dim>
using bounds_t = stencil::bounds_t<cell_t, Dim>;

// stencil code using jacobi iteration
using stencil_code = stencil::stencil_iteration<stencil::jacobi_iteration>;

// ==============================================================================
// entry point

template<size_t Dim>
bounds_t<Dim> parse_bounds(const int argc, char* argv[], const size_t offset)
{
    bounds_t<Dim> bounds;

    if (argc < static_cast<int>(offset + bounds.size()))
    {
        throw std::runtime_error("bounds missing ( " + 
            std::to_string(argc - offset) + 
            " instead of " + 
            std::to_string(bounds.size()) + 
            " )");
    }

    for (auto i = 0u; i < bounds.size(); ++i)
        bounds[i] = { 
            static_cast<cell_value_t>(atof(argv[offset + i])), 
            static_cast<cell_value_t>(atof(argv[offset + i])) 
        };

    return bounds;
}

template<size_t Dim>
void execute_stencil_code(const float epsilon, 
    const stencil::grid_extents_t<Dim> extents, 
    const bounds_t<Dim> bounds)
{
    chrono_timer timer("create (" + std::to_string(Dim) + "D, " +
        std::to_string(stencil::size(extents)) + ")");

    auto grid = create_grid<cell_t, Dim>(extents, bounds, {0, 0});

    timer.print();  
    timer.stop();

    if (extents[0] < 50)
    {
        std::cout << "\n" << "Start Configuration: " << "\n\n";

        print(grid);
        std::cout << std::endl;
    }

    auto final_value_index = 0;
    auto iterations = 0;

    {
        chrono_timer t("jacobi (" + std::to_string(Dim) + "D, " + 
            std::to_string(stencil::size(extents)) + ")");

        for(;;)
        {
            ++iterations; 
            if (stencil_code::do_iteration<0>(grid) < epsilon)
            {
                final_value_index = 1;
                break;
            }

            ++iterations;
            if (stencil_code::do_iteration<1>(grid) < epsilon)
                break;
        }
    }

    std::cout << "iterations: " << iterations << std::endl;

    // hacky - copies the final values into the first
    // element (convenience only)
    if (final_value_index == 1) stencil_code::do_iteration<1>(grid);

    if (extents[0] > 50) return;

    std::cout << "\n" << "Result: " << "\n\n";
    print(grid);
    std::cout << std::endl;
}

int main(const int argc, char* argv[])
{

#ifdef _MSC_VER
    struct do_finally { ~do_finally() { std::cin.get(); } } _;
#endif

    if (argc < 6 )
    {
        std::cout << "usage: <epsilon> <dim> <n> <left_bound> <right_bound>" 
                  << "[<top_bound> <bottom_bound> [<front_bound> <back_bound>]]" 
                  << std::endl;

        return EXIT_FAILURE;
    }

    const auto epsilon = static_cast<cell_value_t>(atof(argv[1]));
    const auto dim = atoi(argv[2]);
    const auto n = static_cast<size_t>(atoi(argv[3]));

    if(epsilon <= 0)
    {
        std::cerr << "epsilon must be greater than 0";
        return EXIT_FAILURE;
    }

    if (n <= 0)
    {
        std::cerr << "n must be greater than 0";
        return EXIT_FAILURE;
    }

    if(dim == 0 || dim > 3)
    {
        std::cerr << "dim must be either 1, 2 or 3";
        return EXIT_FAILURE;
    }

    constexpr auto bounds_arg_offset = 4;

    try
    {
        switch (dim)
        {
        case 1: execute_stencil_code<1>(epsilon, { n }, 
            parse_bounds<1>(argc, argv, bounds_arg_offset)); break;

        case 2: execute_stencil_code<2>(epsilon, { n, n }, 
            parse_bounds<2>(argc, argv, bounds_arg_offset)); break;

        case 3: execute_stencil_code<3>(epsilon, { n, n, n },
            parse_bounds<3>(argc, argv, bounds_arg_offset)); break;

        default: return EXIT_FAILURE;
        }
    }
    catch(const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
