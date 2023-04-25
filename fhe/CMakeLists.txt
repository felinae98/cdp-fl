cmake_minimum_required(VERSION 3.13)
project(cdp_fl VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

set(TFHE_PATH "${CMAKE_SOURCE_DIR}/tfhe" CACHE PATH "TFHE library path" FORCE)
include_directories("${CMAKE_SOURCE_DIR}/Cingulata/common/include")
include_directories("${CMAKE_SOURCE_DIR}/Cingulata/runtime/tfhe/include")
add_subdirectory(Cingulata)

add_executable(init-one-zero src/init_one_zero.cpp)
target_link_libraries(init-one-zero common tfhe_bit_exec)

include_directories("${CMAKE_SOURCE_DIR}/include")
add_library(cdp-fl-utils src/utils.cpp)

add_executable(test-dec-enc src/test_enc_dec.cpp)
target_link_libraries(test-dec-enc common tfhe_bit_exec cdp-fl-utils)

include_directories("${CMAKE_SOURCE_DIR}/tfhe/src/include")
add_subdirectory(thtfhe)

include_directories("${CMAKE_SOURCE_DIR}/thtfhe/include")
add_executable(test-vanilla-dec src/test_vanilla_dec.cpp)
target_link_libraries(test-vanilla-dec ${TFHE_LIBRARIES} thfhe cblas)