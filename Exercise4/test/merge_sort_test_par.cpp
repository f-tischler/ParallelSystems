//
// Created by ivan on 03/11/17.
//

#define PAR_OPT


#include "../merge_sort.h"
#include "../chrono_timer.h"
#include <cstdlib>
#include <iostream>


int main(int args, char** argv){

    if(args==2) {
        std::cout<<"Num Threads: "<<omp_get_num_threads() <<"   max threads: "<< omp_get_max_threads()<<std::endl;
        std::vector<double> _arr = init(atoi(argv[1]));
        {
            ChronoTimer t("Sort");
            _arr = sort(_arr);
        }
        if (is_sorted(_arr)) return EXIT_SUCCESS;
        else return EXIT_FAILURE;
    } else {
        std::cout<<"Usage: ./merge_sort [num_of_elements]"<<std::endl;
        return EXIT_FAILURE;
    }
}