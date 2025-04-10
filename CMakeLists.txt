cmake_minimum_required(VERSION 3.16)
project(moj)
set(CMAKE_CXX_STANDARD 17)

find_package(LLVM 17 REQUIRED CONFIG)

file(GLOB_RECURSE TARGET_SRC "src/*.cpp" "src/*.h")

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Optional: pick only the components you use (or leave empty for all)
# set(LLVM_COMPONENTS support core irreader)
# llvm_map_components_to_libnames(LLVM_LIBS ${LLVM_COMPONENTS})

add_executable(moj ${TARGET_SRC} main.cpp)

target_include_directories(moj PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(moj PRIVATE ${LLVM_DEFINITIONS})
target_link_libraries(moj PRIVATE LLVM)
