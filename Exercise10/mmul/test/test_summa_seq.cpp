#include "mmul.h"
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    auto num_procs = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    auto rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0)
    {
        MPI_Finalize();
        return EXIT_SUCCESS;
    }

    const auto n = 128;

    const auto a = factory_t::create_index_range(n);
    const auto b = factory_t::create_identity(n);

    const auto c = multiply(a, b, 
        summa_sequential{ gsl::narrow<size_t>(num_procs), false });

    MPI_Finalize();

    Expects(c == a);

    return c == a 
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}
