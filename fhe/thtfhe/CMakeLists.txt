include_directories(include)

find_package(BLAS REQUIRED)
message(STATUS "\t${BLAS_LIBRARIES}")
message(STATUS "\t${BLAS_LINKER_FLAGS}")

add_library(thfhe src/libthfhe.cpp src/threshold_decryption_functions.cpp)
target_include_directories(thfhe PUBLIC ${TFHE_INCLUDE_DIR})
target_link_libraries(thfhe ${TFHE_LIBRARIES})

add_executable(keygen-t src/KeyGen.cpp)
target_include_directories(keygen-t PUBLIC ${TFHE_INCLUDE_DIR})
target_link_libraries(keygen-t ${TFHE_LIBRARIES} thfhe cblas)

add_executable(convert-t src/Convert.cpp)
target_include_directories(convert-t PUBLIC ${TFHE_INCLUDE_DIR})
target_link_libraries(convert-t ${TFHE_LIBRARIES} thfhe cblas)
