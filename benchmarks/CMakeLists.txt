find_package(benchmark CONFIG REQUIRED)

add_executable(trivial-bench trivial.cpp)
target_link_libraries(trivial-bench PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_compile_features(trivial-bench PRIVATE cxx_std_17)
