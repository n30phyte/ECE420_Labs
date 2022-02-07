add_executable(Lab2_client Code/client.c)
add_executable(Lab2_server1 Code/server1.cpp)

add_executable(Lab2_server Code/simpleServer.c)
add_executable(Lab2_simpleclient Code/simpleClient.c)


#add_executable(Lab2_main2 Code/serialtester.c Code/IO.c)
#add_executable(Lab2_main3 Code/matrixgen.c)
#add_executable(Lab2_main4 Code/serialtester.c Code/IO.c)
#add_executable(Lab2_mainK Code/serialtester.c Code/IO.c)

target_link_libraries(Lab2_server1 Threads::Threads)

target_link_libraries(Lab2_server Threads::Threads)
target_link_libraries(Lab2_simpleclient Threads::Threads)