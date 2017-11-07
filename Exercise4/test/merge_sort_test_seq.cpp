//
// Created by ivan on 03/11/17.
//

#define SEQ_OPT

#include "../merge_sort.h"
#include "../chrono_timer.h"
#include <cstdlib>

int main(int args, char** argv)
{
    constexpr auto problem_size = 100000;

    auto arr = init(problem_size);
    {
        ChronoTimer t("Start");
        arr = sort(arr);
    }


    return is_sorted(arr)
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}