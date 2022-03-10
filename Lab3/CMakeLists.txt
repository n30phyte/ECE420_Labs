find_package(OpenMP REQUIRED)

add_executable(Lab3_datagen Code/datagen.c Code/IO.c)
add_executable(Lab3_serialtester Code/serialtester.c Code/IO.c)

add_executable(Lab3_main Code/main.cpp Code/IO.c)

target_link_libraries(Lab3_main PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(Lab3_serialtester PRIVATE m)
