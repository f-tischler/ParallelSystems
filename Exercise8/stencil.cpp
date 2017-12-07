//
// Created by ivan on 04/12/17.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <mpi.h>
#include <cmath>
#include "grid.h"
#include "mpi_stencil.h"

constexpr auto epsilon = 20;

int main(int argc, char **argv) 
{
    const auto borders = new double[4];

    borders[0] = 1.0;
    borders[1] = 0.5;
    borders[2] = 0;
    borders[3] = -0.5;

    auto grid1 = new grid(borders);
    auto grid2 = new grid(borders);

    unsigned iterations = 0;
    unsigned sum_iterations;
    
    //Start to measure time
    using clock = std::chrono::high_resolution_clock;
    const auto start = clock::now();

    MPI_Init(&argc, &argv);

    auto num_procs = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    auto my_rank = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    const auto blocks = static_cast<int>(sqrt(num_procs));

    if( size % num_procs != 0 && blocks * blocks != num_procs) 
    {
        if (my_rank == 0)
        {
            std::cerr << "Matrix size not divisible by number of processors: " 
                      << num_procs << std::endl;
        }
           
        MPI_Finalize();
        
        return EXIT_FAILURE;
    }

    grid1->set_block(my_rank, sqrt(num_procs));
    grid2->set_block(my_rank, sqrt(num_procs));

    stencil s(num_procs, my_rank, grid1);
    
    // Always do two iterations, as the arrays have to switch every time.
    // This way, it is not needed to keep track which was the last
    while (true) 
    {
        s.iteration(grid1, grid2);
        s.send_recv_border(grid2);

        const auto d_epsilon = s.iteration(grid2, grid1);
        s.send_recv_border(grid1);

        iterations+=2;

        double sum_epsilon;
        MPI_Allreduce(&d_epsilon, &sum_epsilon, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        if (sum_epsilon < epsilon) 
            break;
    }

    MPI_Reduce(&iterations,&sum_iterations,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

    MPI_Finalize();

    if (my_rank != 0) return EXIT_SUCCESS;

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            clock::now() - start).count();

    std::cout << elapsed << " ms, " << iterations << " iter";

    if(elapsed > 0)
    {
        std::cout << ", " << std::scientific << std::setprecision(3)
                    << iterations * size / static_cast<double>(elapsed) * 1000
                    << " cells/s";
    }

    std::cout << std::endl;
   
    return EXIT_SUCCESS;
}

