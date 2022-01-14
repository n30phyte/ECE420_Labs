add_executable(Lab1 Code/main.cpp Code/IO.c)

add_executable(Lab1_MatrixGen Code/matrixgen.c)
add_executable(Lab1_SerialTester Code/serialtester.c Code/IO.c)

target_link_libraries(Lab1 Threads::Threads)