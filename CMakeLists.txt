cmake_minimum_required(VERSION 3.14)
project(SimuladorRegla110 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar y vincular OpenMP
find_package(OpenMP REQUIRED)

add_executable(sim_regla110
    main.cpp
    TuringMachine.cpp
    Rule110.cpp
)

target_link_libraries(sim_regla110 PRIVATE OpenMP::OpenMP_CXX)