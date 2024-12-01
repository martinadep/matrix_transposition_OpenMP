cmake_minimum_required(VERSION 3.29)
project(PARCO1 C)

set(CMAKE_C_STANDARD 11)

# main executable
add_executable(main src/main.c test_functions/test_functions.c src/main.h src/utils.c)
# block based executable
add_executable(implicit implicit/implicit.c implicit/implicit.h src/utils.c)
# block based executable
add_executable(blocksize implicit/block_size.c implicit/implicit.h src/utils.c)
# explicit executable
add_executable(open explicit/explicit.c explicit/explicit.h src/utils.c)

# Include directories for shared headers
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(implicit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(blocksize PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(open PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


set(CMAKE_C_FLAGS  "-fopenmp")
#set(CMAKE_C_FLAGS "-O1")
#set(CMAKE_C_FLAGS "-O2")
#set(CMAKE_C_FLAGS "-O1 -march=native")
#set(CMAKE_C_FLAGS "-O2 -march=native")

