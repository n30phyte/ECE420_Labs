add_executable(Lab2_client Code/client.c Code/common.c)

add_executable(Lab2_server1 Code/server1.cpp Code/server.cpp Code/common.c)
add_executable(Lab2_server2 Code/server2.cpp Code/server.cpp Code/common.c)
add_executable(Lab2_server3 Code/server3.cpp Code/server.cpp Code/common.c)
add_executable(Lab2_server4 Code/server4.cpp Code/server.cpp Code/common.c)

add_executable(Lab2_attacker Code/attacker.c Code/common.c)

target_link_libraries(Lab2_client Threads::Threads)
target_link_libraries(Lab2_attacker Threads::Threads m)

target_link_libraries(Lab2_server1 Threads::Threads)
target_link_libraries(Lab2_server2 Threads::Threads)
target_link_libraries(Lab2_server3 Threads::Threads)
target_link_libraries(Lab2_server4 Threads::Threads)
