add_library(simulation_lib
    grid.cpp
    mouse.cpp
    simulator.cpp
)

target_include_directories(simulation_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
