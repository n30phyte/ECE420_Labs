add_executable(Lab2_client Code/client.c)
add_executable(Lab2_server1 Code/server1.cpp)
add_executable(Lab2_server2 Code/server2.cpp)

target_link_libraries(Lab2_client Threads::Threads)
target_link_libraries(Lab2_server1 Threads::Threads)
target_link_libraries(Lab2_server2 Threads::Threads)
