add_executable(Lab2_client Code/client.c Code/common.c)
add_executable(Lab2_attacker Code/attacker.c Code/common.c)

add_executable(Lab2_main1 Code/server_impl.cpp Code/server.cpp Code/common.c)
add_executable(Lab2_main2 Code/server_impl.cpp Code/server.cpp Code/common.c)
add_executable(Lab2_main3 Code/server_impl.cpp Code/server.cpp Code/common.c)
add_executable(Lab2_main4 Code/server_impl.cpp Code/server.cpp Code/common.c)

target_link_libraries(Lab2_client Threads::Threads)
target_link_libraries(Lab2_attacker Threads::Threads m)

target_link_libraries(Lab2_main1 Threads::Threads)
target_link_libraries(Lab2_main2 Threads::Threads)
target_link_libraries(Lab2_main3 Threads::Threads)
target_link_libraries(Lab2_main4 Threads::Threads)

target_compile_definitions(Lab2_main1 PRIVATE SINGLE_MUTEX)
target_compile_definitions(Lab2_main2 PRIVATE MULTI_MUTEX)
target_compile_definitions(Lab2_main3 PRIVATE SINGLE_RWLOCK)
target_compile_definitions(Lab2_main4 PRIVATE MULTI_RWLOCK)