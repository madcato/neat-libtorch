cmake_minimum_required (VERSION 3.0 FATAL_ERROR)
project (neat-libtorch)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH /opt/libtorch/share/cmake/Torch)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
include_directories(SYSTEM ${TORCH_INCLUDE_DIRS})

find_package(CUDA)
if(CUDA_FOUND)
  include_directories(${CUDA_INCLUDE_DIRS})
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCUDA_AVAILABLE")
  add_definitions(-DCUDA_LIB_AVAILABLE)
endif()

include_directories(${CMAKE_SOURCE_DIR})

# Main executable
add_executable(neat-libtorch neat/main.cpp
                             neat/activations.cpp
                             neat/mutation.cpp
                             neat/unit.cpp
                             neat/utils.cpp
                             neat/genotype/connection_gene.cpp
                             neat/genotype/genome.cpp
                             neat/genotype/node_gene.cpp
                             neat/phenotype/feed_forward.cpp)
target_link_libraries(neat-libtorch "${TORCH_LIBRARIES}")
set_property(TARGET neat-libtorch PROPERTY CXX_STANDARD 14)

# System Information
add_executable(system-info sample/system_info.cpp)
target_link_libraries(system-info "${TORCH_LIBRARIES}")
set_property(TARGET system-info PROPERTY CXX_STANDARD 14)