add_executable(Lab2_client Code/client.c Code/common.c)
add_executable(Lab2_attacker Code/attacker.c Code/common.c)

add_executable(Lab2_main1 Code/main1.cpp Code/server.cpp Code/common.c)
add_executable(Lab2_main2 Code/main2.cpp Code/server.cpp Code/common.c)
add_executable(Lab2_main3 Code/main3.cpp Code/server.cpp Code/common.c)
add_executable(Lab2_main4 Code/main4.cpp Code/server.cpp Code/common.c)

target_link_libraries(Lab2_client Threads::Threads)
target_link_libraries(Lab2_attacker Threads::Threads m)

target_link_libraries(Lab2_main1 Threads::Threads)
target_link_libraries(Lab2_main2 Threads::Threads)
target_link_libraries(Lab2_main3 Threads::Threads)
target_link_libraries(Lab2_main4 Threads::Threads)