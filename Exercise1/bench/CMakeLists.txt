find_package(Threads REQUIRED)

#~~~~~~~~~~~~~~~~

file(GLOB_RECURSE ALL_BENCH_CPP *.cpp)
 
foreach(ONE_BENCH_CPP ${ALL_BENCH_CPP})

   get_filename_component(ONE_BENCH_EXEC ${ONE_BENCH_CPP} NAME_WE)
   get_filename_component(ONE_BENCH_EXEC_DIR ${ONE_BENCH_CPP} DIRECTORY)

   set(LOG_NAME ${ONE_BENCH_EXEC_DIR}/${ONE_BENCH_EXEC}.lastrun.json)
   set(TARGET_NAME Benchmark_${ONE_BENCH_EXEC})

   add_executable(${TARGET_NAME} ${ONE_BENCH_CPP})
   set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${ONE_BENCH_EXEC}) 
   
   target_link_libraries(${TARGET_NAME} 
		benchmark
		${CMAKE_THREAD_LIBS_INIT})

   add_test(NAME ${TARGET_NAME}
		    COMMAND ${TARGET_NAME} --benchmark_out=${LOG_NAME} 
								   --benchmark_out_format=json)
 
endforeach()