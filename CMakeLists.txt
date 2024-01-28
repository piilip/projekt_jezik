cmake_minimum_required(VERSION 3.16)

project(moj)
set(CMAKE_CXX_STANDARD 17)


find_package(LLVM 17 REQUIRED CONFIG)

add_link_options("-fPIC")
file(GLOB_RECURSE TARGET_SRC "src/*.cpp" "src/*.h")

set(LLVM_LIBS "-lLLVM-17")


message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Using LLDConfig.cmake in: ${LLD_DIR}")
message(STATUS "libs: ${LLD_LIBS} ${LLVM_LIBS}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_executable(moj ${TARGET_SRC} main.cpp)

target_link_libraries(moj ${LLD_LIBS} ${LLVM_LIBS})
